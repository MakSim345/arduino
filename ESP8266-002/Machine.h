#ifndef MACHINE_H
#define MACHINE_H

#include "datastore.h"

// Demo pump: dims a pin (onboard LED) with PWM and prints to Serial.
// pwm is 0..1023 (see DataStore::strToPumpPwm).
class Pump {
  public:
  Pump(int pin, bool activeLow = false): pin(pin), activeLow(activeLow) {
  }
  void begin() {
    pinMode(pin, OUTPUT);
    analogWriteRange(1023);          // ESP8266 core 3.x defaults to 255
    off();
  }
  void on(int pwm) {
    analogWrite(pin, activeLow ? 1023 - pwm : pwm);
    Serial.printf("Pump ON (pwm %d)\n", pwm);
  }
  void off() {
    analogWrite(pin, 0);             // stop PWM before plain digitalWrite
    digitalWrite(pin, activeLow ? HIGH : LOW);
    Serial.println("Pump OFF");
  }
  private:
  int pin;
  bool activeLow;
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
