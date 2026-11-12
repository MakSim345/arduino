// WeMos D1 R1 (ESP8266 / ESP-12F, Arduino UNO form factor, CH340G USB)
// Arduino IDE: Tools > Board > ESP8266 Boards > "LOLIN(WeMos) D1 R1"
//
// NOTE: on this board the labels D0/D1 are RX/TX (Serial!), so pins
// are given as raw GPIO numbers. Board label <-> GPIO:
//   GPIO16 = D2          GPIO5  = D15/SCL/D3    GPIO4  = D14/SDA/D4
//   GPIO14 = D13/SCK/D5  GPIO12 = D12/MISO/D6   GPIO13 = D11/MOSI/D7
//   GPIO0  = D8          GPIO2  = TX1/D9 (blue LED on ESP-12F, active LOW)
//   GPIO15 = D10/SS      A0     = analog input (max 3.3V on this board)
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include "secrets.h"   // WiFi list, not in git: copy secrets.example.h -> secrets.h
#include "datastore.h"
#include "AppServer.h"
#include "Machine.h"

ESP8266WiFiMulti wifiMulti;
DataStore datastore = DataStore();
AppServer webServer = AppServer(datastore);
Pump pump = Pump(2, true);             // GPIO2 = blue LED on ESP-12F (active LOW)
Router router = Router(5, 16, 14, 12); // dummy, pins are not used (D15, D2, D13, D12)
Machine machine = Machine(router, pump, datastore);

void setup()
{
  Serial.begin(115200);
  delay(500);
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.hostname("esp8266");
  for (int i = 0; i < WIFI_NETWORKS_COUNT; i++) {
    wifiMulti.addAP(WIFI_NETWORKS[i].ssid, WIFI_NETWORKS[i].password);
    Serial.print("Known network: ");
    Serial.println(WIFI_NETWORKS[i].ssid);
  }
  Serial.print("Connecting");

  unsigned long start = millis();
  while (wifiMulti.run() != WL_CONNECTED)  // picks the strongest known network
  {
    Serial.print(".");
    delay(500);                      // delay() also feeds the ESP8266 watchdog
    if (millis() - start > 30000) {  // give up after 30 s and retry
      Serial.println("\nCan't connect, check secrets.h. Restarting...");
      ESP.restart();
    }
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // lets you open http://esp8266.local instead of typing the IP
  if (MDNS.begin("esp8266")) {
    MDNS.addService("http", "tcp", 80);
    Serial.println("Or open: http://esp8266.local");
  }

  datastore.begin();
  webServer.begin();
  pump.begin();
  router.begin();
}

void loop()
{
  wifiMulti.run();                   // reconnects (to any known network) if WiFi drops
  MDNS.update();                     // ESP8266 mDNS must be serviced in loop
  machine.handle();
  webServer.handle();
  datastore.handle();
}
