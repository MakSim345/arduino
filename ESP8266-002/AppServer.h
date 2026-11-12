#ifndef APP_SERVER_H
#define APP_SERVER_H

#include "datastore.h"
#include <ESP8266WebServer.h>

class AppServer {
  public:
  AppServer(DataStore& datastore ): dataHandler(datastore) {
  }
  void begin() {
    serv.on("/", HTTP_GET, std::bind(&AppServer::handleRootPath, this));
    serv.on("/save", HTTP_POST, std::bind(&AppServer::handleSaveSettings, this));
    serv.on("/testall",HTTP_GET, std::bind(&AppServer::handleTestAll, this));
    serv.begin();
    Serial.println("Web server started");
  }
  void handle() {
    serv.handleClient();
  }

  private:
  ESP8266WebServer serv{80};
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
    Serial.println("Settings saved");
    goHome();
  }
  void handleTestAll() {
    Serial.println("test");
    dataHandler.setWateringNow();
    goHome();
  }
  // send the browser back to the main page
  void goHome() {
    serv.sendHeader("Location", "/");
    serv.send(303);
  }

};
#endif
