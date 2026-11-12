#ifndef MACHINE_H
#define MACHINE_H

#include "datastore.h"

// Dummy pump: just switches a pin (onboard LED) and prints to Serial
class Pump {
  public:
  Pump(int pin): pin(pin) {
  }
  void begin() {
    pinMode(pin, OUTPUT);
    off();
  }
  void on(int pwm) {
    digitalWrite(pin, HIGH);
    Serial.printf("Pump ON (pwm %d)\n", pwm);
  }
  void off() {
    digitalWrite(pin, LOW);
    Serial.println("Pump OFF");
  }
  private:
  int pin;
};

// Dummy router (valve selector): only prints which channel is selected
class Router {
  public:
  Router(int a, int b, int c, int d) {
  }
  void begin() {
  }
  void select(int channel) {
    Serial.printf("Router -> channel %d\n", channel + 1);
  }
};

// Waters channels 1..6 one after another when "Test all" is pressed.
// Uses millis(), so the web server keeps working meanwhile.
class Machine {
  public:
  Machine(Router& router, Pump& pump, DataStore& datastore)
    : router(router), pump(pump), datastore(datastore) {
  }
  void handle() {
    if (channel < 0) {                 // idle
      if (datastore.isWateringNow())
        startChannel(0);
      return;
    }
    if (millis() - startedAt < (unsigned long)datastore.getWateringTime(channel))
      return;                          // current channel still running
    pump.off();
    if (channel < 5) {
      startChannel(channel + 1);
    } else {
      channel = -1;
      datastore.clearWateringNow();
      Serial.println("Watering done");
    }
  }

  private:
  void startChannel(int ch) {
    channel = ch;
    startedAt = millis();
    router.select(ch);
    pump.on(datastore.getPumpPwm());
  }

  Router& router;
  Pump& pump;
  DataStore& datastore;
  int channel = -1;
  unsigned long startedAt = 0;
};
#endif
