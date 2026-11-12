// WeMos D1 R1 (ESP8266) + DS3231 RTC module - measure RTC drift against NTP.
// Arduino IDE: Tools > Board > ESP8266 Boards > "LOLIN(WeMos) D1 R1"
//
// Wiring (power the module from 3.3V, NOT 5V: its I2C pull-ups go to VCC
// and the ESP8266 pins are not 5V tolerant):
//   DS3231 VCC -> 3.3V
//   DS3231 GND -> GND
//   DS3231 SDA -> GPIO4 (D14/SDA/D4)
//   DS3231 SCL -> GPIO5 (D15/SCL/D3)
//
// On start: set the ESP internal clock from the DS3231. Nothing is set from
// the internet, so the RTC can be left on its battery for weeks and checked later.
// Every minute: catch the moment the RTC seconds change and compare that
// moment with NTP time (read only) -> RTC error with sub-millisecond resolution.
//
// Serial commands (115200 baud, any case):
//   sync - set the DS3231 and the ESP internal clock once from internet time
//          (Helsinki local time, exactly at the start of a second). The sync
//          moment is saved in flash, so the drift (ppm) since that sync is
//          still known after a power-off.
//   help - list commands
//
// The RTC has no idea of summer time: it keeps the UTC offset that was valid
// at SYNC (UTC+3 in summer, UTC+2 in winter). The offset is saved with the
// sync, so the drift stays correct; send SYNC after the clocks change
// (last Sunday of March / October) to get the RTC showing wall time again.
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <RTClib.h>
#include <EEPROM.h>
#include <time.h>
#include <sys/time.h>
#include "secrets.h"   // WiFi list, not in git: copy secrets.example.h -> secrets.h

ESP8266WiFiMulti wifiMulti;
WiFiUDP udp;
RTC_DS3231 rtc;
const char* ntpServer = "pool.ntp.org";
IPAddress ntpIP;       // resolved once, so every query goes to the same server
const int NTP_PORT = 123;
const int NTP_SAMPLES = 4;  // queries per measurement, the one with the shortest round trip wins

unsigned long lastPrint = 0;
// const unsigned long interval = 1000;
const unsigned long interval = 60UL * 1000UL; // 1 minute
// const unsigned long interval = 30UL * 60UL * 1000UL; // 30 minutes

// Helsinki: EET = UTC+2, summer time EEST = UTC+3 (last Sun of March 03:00 .. last Sun of October 04:00)
const char* TIMEZONE = "EET-2EEST,M3.5.0/3,M10.5.0/4";
long rtcOffsetS = 0;        // local time - UTC (s), the offset written into the RTC

// one comparison of the RTC against NTP
struct Measurement {
  uint32_t rtcSec;          // RTC reading (local time, unix seconds) right after its tick
  int64_t trueUs;           // NTP (true) time of that tick, UTC us
  int64_t rtcDiffUs;        // RTC error, + = RTC ahead
  int64_t espDiffUs;        // ESP clock error, only used to time the RTC tick
  unsigned long rtt;        // NTP round trip (us)
};

// Saved in flash at SYNC. The setting itself is only as exact as the NTP
// round trip allows (asymmetric WiFi delay can easily be tens of ms), so the
// RTC error measured right after SYNC is kept as baseline and drift is the
// change against it.
struct SavedSync {
  uint32_t magic;           // SYNC_MAGIC = data valid
  int32_t rtcOffsetS;       // local time - UTC written into the RTC
  int64_t syncUs;           // true time of the baseline measurement, UTC us
  int64_t rtcDiffUs;        // RTC error right after SYNC
};
const uint32_t SYNC_MAGIC = 0x52544331;  // "RTC1"
SavedSync saved;

String serialLine;          // command being typed

// local system clock in microseconds since 1970
int64_t nowUs()
{
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  return (int64_t)tv.tv_sec * 1000000LL + tv.tv_usec;
}

// "YYYY-MM-DD hh:mm:ss.mmm" (UTC)
void formatTime(int64_t us, char* buf, size_t len)
{
  time_t t = us / 1000000LL;
  struct tm* tm_ = gmtime(&t);
  snprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
           tm_->tm_year + 1900, tm_->tm_mon + 1, tm_->tm_mday,
           tm_->tm_hour, tm_->tm_min, tm_->tm_sec, (int)((us / 1000) % 1000));
}

// Manual NTP query - does NOT touch system clock.
// Returns NTP time in us (corrected by half the round trip), 0 on failure.
// localUs = local clock at the moment the reply arrived.
int64_t getNtpTimeUs(int64_t& localUs, unsigned long& roundTripUs)
{
  byte packet[48] = {0};
  packet[0] = 0b11100011; // LI, Version, Mode

  while (udp.parsePacket() > 0)  // drop late replies of earlier queries
    udp.flush();

  unsigned long sentAt = micros();
  udp.beginPacket(ntpIP, NTP_PORT);
  udp.write(packet, 48);
  udp.endPacket();

  while (micros() - sentAt < 3000000UL)
  {
    if (udp.parsePacket() >= 48)
    {
      localUs = nowUs();
      roundTripUs = micros() - sentAt;
      udp.read(packet, 48);
      // transmit timestamp: seconds (40..43) and fraction of second (44..47)
      uint32_t secsSince1900 = (uint32_t)packet[40] << 24 | (uint32_t)packet[41] << 16 |
                               (uint32_t)packet[42] << 8 | packet[43];
      uint32_t fraction = (uint32_t)packet[44] << 24 | (uint32_t)packet[45] << 16 |
                          (uint32_t)packet[46] << 8 | packet[47];
      const uint32_t seventyYears = 2208988800UL;
      int64_t us = (int64_t)(secsSince1900 - seventyYears) * 1000000LL +
                   (((uint64_t)fraction * 1000000ULL) >> 32);
      return us + roundTripUs / 2;
    }
    delay(1);
  }
  return 0; // failed
}

// Several NTP queries, keeps the one with the shortest round trip (least WiFi jitter).
int64_t getBestNtpTimeUs(int64_t& localUs, unsigned long& roundTripUs)
{
  int64_t best = 0;
  for (int i = 0; i < NTP_SAMPLES; i++)
  {
    int64_t l;
    unsigned long rtt;
    int64_t ntp = getNtpTimeUs(l, rtt);
    if (ntp != 0 && (best == 0 || rtt < roundTripUs))
    {
      best = ntp;
      localUs = l;
      roundTripUs = rtt;
    }
    if (i < NTP_SAMPLES - 1)
      delay(2000);  // be polite to the NTP server
  }
  return best;
}

// Waits for the next RTC second tick. Returns the RTC time (unix seconds)
// right after the tick and sets tickLocalUs to the local clock at the tick.
// Returns 0 if the RTC does not tick (not running / not connected).
uint32_t waitRtcTick(int64_t& tickLocalUs)
{
  int64_t prevUs = nowUs();
  uint32_t first = rtc.now().unixtime();
  while (nowUs() - prevUs < 2000000LL)
  {
    int64_t us = nowUs();   // DS3231 latches its time at the start of the read
    uint32_t t = rtc.now().unixtime();
    if (t != first)
    {
      tickLocalUs = (prevUs + us) / 2;  // tick happened between the two reads
      return t;
    }
    prevUs = us;
    yield();                // keep the ESP8266 watchdog and WiFi happy
  }
  return 0;
}

// local time - UTC in seconds at moment t (uses TIMEZONE, incl. summer time)
long utcOffsetS(time_t t)
{
  struct tm lt, gt;
  localtime_r(&t, &lt);
  gmtime_r(&t, &gt);
  long offset = (lt.tm_hour - gt.tm_hour) * 3600L + (lt.tm_min - gt.tm_min) * 60L;
  int days = lt.tm_yday - gt.tm_yday;
  if (days == 1 || days < -1)        // local date is one day later (also over new year)
    offset += 86400L;
  else if (days == -1 || days > 1)   // local date is one day earlier
    offset -= 86400L;
  return offset;
}

// Writes the (NTP-synced) ESP clock as local time into the DS3231 exactly at a whole second.
// Writing the seconds register resets the DS3231 internal countdown chain,
// so the RTC second starts at the moment of writing.
void setRtcFromClock()
{
  int64_t target = (nowUs() / 1000000LL + 2) * 1000000LL;  // start of a second, >= 1 s ahead
  time_t targetS = target / 1000000LL;
  rtcOffsetS = utcOffsetS(targetS);
  while (nowUs() < target)
  {
    if (target - nowUs() > 5000)
      yield();              // no yield in the last 5 ms: keep the timing tight
  }
  rtc.adjust(DateTime((uint32_t)(targetS + rtcOffsetS)));

  char buf[24];
  formatTime((int64_t)(targetS + rtcOffsetS) * 1000000LL, buf, sizeof(buf));
  Serial.printf("RTC set to %s (Helsinki, UTC%+ld)\n", buf, rtcOffsetS / 3600);
}

// Catches an RTC tick, then asks NTP. false on failure.
bool measure(Measurement& m)
{
  int64_t tickLocalUs;
  m.rtcSec = waitRtcTick(tickLocalUs);
  if (m.rtcSec == 0)
  {
    Serial.println("RTC is not ticking, check wiring!");
    return false;
  }

  int64_t localUs;
  int64_t ntpUs = getBestNtpTimeUs(localUs, m.rtt);
  if (ntpUs == 0)
  {
    Serial.println("NTP query failed");
    return false;
  }

  // ESP clock error, then use it to convert the RTC tick moment to true time.
  // (the ESP drift during the few seconds between tick and NTP is negligible)
  m.espDiffUs = localUs - ntpUs;
  m.trueUs = tickLocalUs - m.espDiffUs;
  m.rtcDiffUs = ((int64_t)m.rtcSec - rtcOffsetS) * 1000000LL - m.trueUs;
  return true;
}

// Sets the ESP clock from NTP. false on failure.
bool syncEspClock()
{
  int64_t localUs;
  unsigned long rtt;
  int64_t ntpUs = getBestNtpTimeUs(localUs, rtt);
  if (ntpUs == 0)
    return false;

  ntpUs += nowUs() - localUs;  // time passed since the best reply arrived
  struct timeval tv;
  tv.tv_sec = ntpUs / 1000000LL;
  tv.tv_usec = ntpUs % 1000000LL;
  settimeofday(&tv, nullptr);
  Serial.printf("ESP clock synced (round trip %lu ms).\n", rtt / 1000);
  return true;
}

// Sets the ESP clock from the DS3231, at the moment its seconds change.
void setEspClockFromRtc()
{
  int64_t tickLocalUs;
  uint32_t rtcSec = waitRtcTick(tickLocalUs);
  if (rtcSec == 0)
  {
    Serial.println("RTC is not ticking, ESP clock not set!");
    return;
  }
  int64_t us = ((int64_t)rtcSec - rtcOffsetS) * 1000000LL + (nowUs() - tickLocalUs);
  struct timeval tv;
  tv.tv_sec = us / 1000000LL;
  tv.tv_usec = us % 1000000LL;
  settimeofday(&tv, nullptr);

  char buf[24];
  formatTime((int64_t)rtcSec * 1000000LL, buf, sizeof(buf));
  Serial.printf("ESP clock set from RTC: %s\n", buf);
}

// SYNC command: NTP -> ESP clock -> DS3231, then measure and save the baseline.
void doSync()
{
  Serial.println("SYNC: getting NTP time...");
  if (!syncEspClock())
  {
    Serial.println("SYNC failed: no NTP reply. RTC not changed.");
    return;
  }
  setRtcFromClock();

  Measurement m;
  bool ok = false;
  for (int i = 0; i < 3 && !ok; i++)
    ok = measure(m);
  if (!ok)
  {
    saved.magic = 0;
    Serial.println("SYNC: RTC was set, but the baseline measurement failed. Send SYNC again.");
    return;
  }

  saved.magic = SYNC_MAGIC;
  saved.rtcOffsetS = rtcOffsetS;
  saved.syncUs = m.trueUs;
  saved.rtcDiffUs = m.rtcDiffUs;
  EEPROM.put(0, saved);
  EEPROM.commit();
  Serial.printf("SYNC done. Setting error (baseline): RTC %+.3f ms. Saved to flash.\n",
                m.rtcDiffUs / 1000.0);
  lastPrint = millis();
}

void printMeasurement(const Measurement& m)
{
  // both in the RTC's time zone, so they can be compared directly
  char rtcBuf[24], ntpBuf[24];
  formatTime((int64_t)m.rtcSec * 1000000LL, rtcBuf, sizeof(rtcBuf));
  formatTime(m.trueUs + (int64_t)rtcOffsetS * 1000000LL, ntpBuf, sizeof(ntpBuf));
  Serial.printf("RTC: %s  NTP: %s  RTC diff: %+.3f ms", rtcBuf, ntpBuf, m.rtcDiffUs / 1000.0);

  if (saved.magic == SYNC_MAGIC)
  {
    double sinceSyncS = (m.trueUs - saved.syncUs) / 1e6;
    if (sinceSyncS > 1)
    {
      double rtcPpm = (m.rtcDiffUs - saved.rtcDiffUs) / sinceSyncS;  // us per second = ppm
      Serial.printf(" (%+.2f ppm, %+.2f s/day, since SYNC %.2f d)",
                    rtcPpm, rtcPpm * 0.0864, sinceSyncS / 86400.0);
    }
  }
  else
  {
    Serial.print(" (no SYNC saved)");
  }
  Serial.println();
}

void printHelp()
{
  Serial.println("Commands: sync - set RTC and internal clock from internet time, help - this text");
}

void handleCommand(String cmd)
{
  cmd.trim();
  cmd.toLowerCase();
  if (cmd == "sync")
  {
    doSync();
  }
  else if (cmd == "help")
  {
    printHelp();
  }
  else
  {
    Serial.printf("Unknown command '%s'. ", cmd.c_str());
    printHelp();
  }
}

// Collects characters into a line. A command ends with CR/LF, or after a short
// pause, so it works with any "line ending" setting of the Serial Monitor.
void handleSerial()
{
  static unsigned long lastCharAt = 0;
  while (Serial.available())
  {
    char c = Serial.read();
    lastCharAt = millis();
    if (c == '\r' || c == '\n')
    {
      if (serialLine.length() > 0)
        handleCommand(serialLine);
      serialLine = "";
    }
    else if (serialLine.length() < 32)
    {
      serialLine += c;
    }
  }
  if (serialLine.length() > 0 && millis() - lastCharAt > 300)
  {
    handleCommand(serialLine);
    serialLine = "";
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println();

  setenv("TZ", TIMEZONE, 1);
  tzset();

  EEPROM.begin(sizeof(SavedSync));
  EEPROM.get(0, saved);

  // DS3231
  while (!rtc.begin())
  {
    Serial.println("DS3231 not found, check wiring! Retrying...");
    delay(2000);
  }
  Wire.setClock(400000);  // fast I2C = shorter reads = finer tick detection
  if (rtc.lostPower())
  {
    Serial.println("DS3231 had lost power (battery empty or missing?), its time is invalid.");
    saved.magic = 0;
  }

  if (saved.magic == SYNC_MAGIC)
  {
    rtcOffsetS = saved.rtcOffsetS;
    char buf[24];
    formatTime(saved.syncUs + (int64_t)rtcOffsetS * 1000000LL, buf, sizeof(buf));
    Serial.printf("Last SYNC: %s (Helsinki, UTC%+ld)\n", buf, rtcOffsetS / 3600);
  }
  else
  {
    rtcOffsetS = utcOffsetS(rtc.now().unixtime());  // best guess: RTC holds Helsinki time
    Serial.println("No valid SYNC saved, drift can not be calculated. Send 'sync' to set the RTC.");
  }

  // the internal clock takes its time from the RTC, not from the internet
  setEspClockFromRtc();

  // WiFi
  WiFi.mode(WIFI_STA);
  for (int i = 0; i < WIFI_NETWORKS_COUNT; i++)
    wifiMulti.addAP(WIFI_NETWORKS[i].ssid, WIFI_NETWORKS[i].password);
  Serial.print("Connecting");

  while (wifiMulti.run() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  udp.begin(2390);
  while (!WiFi.hostByName(ntpServer, ntpIP))
  {
    Serial.println("DNS lookup failed, retrying...");
    delay(2000);
  }
  Serial.print("NTP server: ");
  Serial.println(ntpIP);

  // first comparison right away (NTP is only read, nothing is set)
  Measurement m;
  while (!measure(m))
    delay(2000);
  printMeasurement(m);
  printHelp();
  lastPrint = millis();
}

void loop()
{
  wifiMulti.run();  // reconnect if WiFi drops
  handleSerial();

  unsigned long nowMillis = millis();
  if (nowMillis - lastPrint < interval)
    return;
  lastPrint = nowMillis;

  Measurement m;
  if (measure(m))
    printMeasurement(m);
}

