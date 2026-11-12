#include <ESP8266WiFi.h>
#include "WebServer.h"
#include "Machine.h"

const char* ssid     = "#####";
const char* password = "****";

DataStore datastore = DataStore();
WebServer webServer = WebServer(datastore);
Pump pump = Pump(D2);
Router router = Router(D1, D0, D5, D6);
Machine machine = Machine(router, pump, datastore);

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  SPIFFS.begin();

  datastore.begin();
  webServer.begin();
  router.begin();
}

void loop()
{
  machine.handle();
  webServer.handle();
  datastore.handle();
}
