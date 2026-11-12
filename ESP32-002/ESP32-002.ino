#include <WiFi.h>
#include <ESPmDNS.h>
#include "datastore.h"
#include "AppServer.h"
#include "Machine.h"

const char* ssid     = "#####";   // your WiFi name (2.4 GHz only!)
const char* password = "****";    // your WiFi password

DataStore datastore = DataStore();
AppServer webServer = AppServer(datastore);
Pump pump = Pump(2);                 // GPIO2 = onboard LED on most ESP32 DevKits
Router router = Router(16, 17, 18, 19); // dummy, pins are not used
Machine machine = Machine(router, pump, datastore);

void setup()
{
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
    if (millis() - start > 20000) {  // give up after 20 s and retry
      Serial.println("\nCan't connect, check SSID/password. Restarting...");
      ESP.restart();
    }
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // lets you open http://esp32.local instead of typing the IP
  if (MDNS.begin("esp32")) {
    Serial.println("Or open: http://esp32.local");
  }

  datastore.begin();
  webServer.begin();
  pump.begin();
  router.begin();
}

void loop()
{
  machine.handle();
  webServer.handle();
  datastore.handle();
}
