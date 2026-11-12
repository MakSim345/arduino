#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiUdp.h>
#include <time.h>
#include "secrets.h"   // WiFi list, not in git: copy secrets.example.h -> secrets.h

WiFiMulti wifiMulti;
WiFiUDP udp;
const char* ntpServer = "pool.ntp.org";
const int NTP_PORT = 123;

unsigned long lastPrint = 0;
// const unsigned long interval = 1000;
const unsigned long interval = 60UL * 1000UL; // 1 minute
// const unsigned long interval = 30UL * 60UL * 1000UL; // 30 minutes

void formatTime(time_t t, char* buf, size_t len)
{
  struct tm* tm_ = gmtime(&t);
  snprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d",
           tm_->tm_year + 1900, tm_->tm_mon + 1, tm_->tm_mday,
           tm_->tm_hour, tm_->tm_min, tm_->tm_sec);
}

// Manual NTP query - does NOT touch system clock
time_t getNtpTime()
{
  byte packet[48] = {0};
  packet[0] = 0b11100011; // LI, Version, Mode

  udp.begin(2390);
  udp.beginPacket(ntpServer, NTP_PORT);
  udp.write(packet, 48);
  udp.endPacket();

  unsigned long start = millis();
  while (millis() - start < 3000)
  {
    if (udp.parsePacket() >= 48)
    {
      udp.read(packet, 48);
      unsigned long highWord = word(packet[40], packet[41]);
      unsigned long lowWord = word(packet[42], packet[43]);
      unsigned long secsSince1900 = (highWord << 16) | lowWord;
      const unsigned long seventyYears = 2208988800UL;
      return secsSince1900 - seventyYears; // epoch (UTC)
    }
  }
  return 0; // failed
}

void setup()
{
  struct tm timeinfo;

  Serial.begin(115200);
  delay(1000);

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

  configTime(0, 0, ntpServer);
  Serial.println("Waiting for NTP time sync...");

  while (!getLocalTime(&timeinfo))
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Time synced.");
}

void loop()
{
  unsigned long nowMillis = millis();
  if (nowMillis - lastPrint >= interval)
  {
    lastPrint = nowMillis;
    time_t rtcTime = time(nullptr);
    char rtcBuf[20];
    formatTime(rtcTime, rtcBuf, sizeof(rtcBuf));

    time_t ntpTime = getNtpTime();

    if (ntpTime == 0)
    {
      Serial.print("RTC: ");
      Serial.print(rtcBuf);
      Serial.println("  NTP query failed");
    }
    else
    {
      char ntpBuf[20];
      formatTime(ntpTime, ntpBuf, sizeof(ntpBuf));
      long diff = (long)(rtcTime - ntpTime);

      Serial.print("RTC: ");
      Serial.print(rtcBuf);
      Serial.print("  NTP: ");
      Serial.print(ntpBuf);
      Serial.print("  Diff (s): ");
      Serial.println(diff);
  }
 }
}
