#ifndef SERVER_H
#define SERVER_H

#include "DataStore.h"
#include <ESP8266WebServer.h>

class WebServer {
  public:
  WebServer(DataStore& datastore ): dataHandler(datastore) {
  }
  void begin() {
    serv.begin();
    serv.on("/", HTTP_GET, std::bind(&WebServer::handleRootPath, this));
    serv.on("/save", HTTP_POST, std::bind(&WebServer::handleSaveSettings, this));
    serv.on("/testall",HTTP_GET, std::bind(&WebServer::handleTestAll, this));
  }
  void handle() {
    serv.handleClient();
  }

  private:
  ESP8266WebServer serv;
  DataStore& dataHandler;

   void handleRootPath() {
      serv.send(200, "text/html", dataHandler.getWebPage());

    }
  void handleSaveSettings() {
    if (serv.hasArg("interval")
    &&serv.hasArg("power")
    &&serv.hasArg("water1")
    &&serv.hasArg("water2")
    &&serv.hasArg("water3")
    &&serv.hasArg("water4")
    &&serv.hasArg("water5")
    &&serv.hasArg("water6"))
      dataHandler.save(serv.arg("interval"),
        serv.arg("power"),
        serv.arg("water1"),
        serv.arg("water2"),
        serv.arg("water3"),
        serv.arg("water4"),
        serv.arg("water5"),
        serv.arg("water6"));
  }
  void handleTestAll() {
    Serial.println("test");
    dataHandler.setWateringNow();
  }

};
#endif
