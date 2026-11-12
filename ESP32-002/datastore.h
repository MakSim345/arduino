#ifndef DATASTORE_H
#define DATASTORE_H

#include <Arduino.h>

// Simplified for ESP32 test: no FTP, no SPIFFS files.
// The web page is built in code and settings live only in RAM
// (they are lost after reset).
class DataStore {
  public:
  DataStore() {
  }
  void begin() {
  }
  const String getWebPage() {
    String page = F(
      "<!DOCTYPE html><html><head><meta charset='utf-8'>"
      "<meta name='viewport' content='width=device-width, initial-scale=1'>"
      "<title>ESP32 Watering</title>"
      "<style>body{font-family:sans-serif;margin:20px}"
      "input{width:80px;font-size:18px}label{display:inline-block;width:160px}"
      "button,a.btn{font-size:18px;padding:8px 16px;margin-top:10px;display:inline-block}</style>"
      "</head><body><h2>ESP32 Watering (demo)</h2>");
    page += "<p>Uptime: " + String(millis() / 1000) + " s | ";
    page += isWateringNowSet ? "<b>Watering now...</b>" : "Idle";
    page += "</p><form method='POST' action='/save'>";
    page += field("interval", "Interval (days)", intervalDays);
    page += field("power", "Pump power (%)", powerPercent);
    for (int i = 0; i < 6; i++) {
      page += field("water" + String(i + 1), "Water " + String(i + 1) + " (s)", waterSeconds[i]);
    }
    page += F("<button type='submit'>Save</button></form>"
              "<a class='btn' href='/testall'>Test all</a>"
              "</body></html>");
    return page;
  }
  void handle() {
  }

  int getInterval() {
    return intervalWatering;
  }
  int getPumpPwm() {
    return pumpPwm;
  }
  long getWateringTime(int index) {
    return wateringTime[index];
  }

  void setWateringNow() {
    isWateringNowSet = true;
  }
  void clearWateringNow() {
    isWateringNowSet = false;
  }
  bool isWateringNow() {
    return isWateringNowSet;
  }

  bool save(
    const String &interval, const String &power,
    const String &water1,const String &water2,const String &water3, const String &water4,const String &water5,const String &water6) {
    const String* water[6] = {&water1, &water2, &water3, &water4, &water5, &water6};
    intervalDays = interval.toInt();
    powerPercent = power.toInt();
    intervalWatering = strToIntervalWatering(interval);
    pumpPwm = strToPumpPwm(power);
    for (int i = 0; i < 6; i++) {
      waterSeconds[i] = water[i]->toInt();
      wateringTime[i] = strToWateringTime(*water[i]);
    }
    return true;
  }
  static unsigned long strToIntervalWatering(const String &interval) {
    return interval.toInt()*86400ul;
  }
  static int strToPumpPwm(const String &power) {
    return (float)(power.toInt()*1023)/100;
  }
  static long strToWateringTime(const String &wateringTime) {
    return wateringTime.toInt()*1000ul;
  }
  private:
  // one "label + number input" row of the form
  static String field(const String& name, const String& label, long value) {
    return "<p><label>" + label + "</label><input type='number' name='" + name +
           "' value='" + String(value) + "'></p>";
  }

  // values as shown on the page
  long intervalDays = 1;
  long powerPercent = 50;
  long waterSeconds[6] = {1, 1, 1, 1, 1, 1};

  // values used by the machine
  unsigned long intervalWatering = 86400;
  int pumpPwm = 512;
  long wateringTime[6] = {1000, 1000, 1000, 1000, 1000, 1000};
  bool isWateringNowSet = false;
};
#endif
