#ifndef DATASTORE_H
#define DATASTORE_H

#include <ESP8266FtpServer.h>

class DataStore {
  public:
  DataStore() {

  }
  void begin() {
    root = "/";
    SPIFFS.begin();

    ftp.begin("engineer", "ftp_fs_begin");
    settings="";
    html = loadWebPageChunk("index.html");
    script = loadWebPageChunk("script.js");
    loadSettings();
  }
  const String getWebPage() {
    String page = html+settings+script;
    if (page="")
    {
      html = loadWebPageChunk("index.html");
      script = loadWebPageChunk("script.js");
      loadSettings();
    }
    return html+settings+script;
  }
  void handle() {
    ftp.handleFTP();
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
    isWateringNowSet = !isWateringNowSet;
  }
  bool isWateringNow() {
    return isWateringNowSet;
  }

  bool save(
    const String &interval, const String &power,
    const String &water1,const String &water2,const String &water3, const String &water4,const String &water5,const String &water6) {
    intervalWatering = strToIntervalWatering(interval);
    pumpPwm = strToPumpPwm(power);
    wateringTime[0] = strToWateringTime(water1);
    wateringTime[1] = strToWateringTime(water2);
    wateringTime[2] = strToWateringTime(water3);
    wateringTime[3] = strToWateringTime(water4);
    wateringTime[4] = strToWateringTime(water5);
    wateringTime[5] = strToWateringTime(water6);
    return storeSettings(
      interval, power,
      water1, water2, water3, water4, water5, water6);
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
  bool storeSettings (
    const String &interval, const String &power,
    const String &water1,const String &water2,const String &water3, const String &water4,const String &water5,const String &water6) {
    String path = root+"settings.js";
    File file = SPIFFS.open(path, "w");
    settings = (String)"<script>\n"
    +"var interval = "+interval+";\n"
    +"var power = "+power+";\n"
    +"var water1 = "+water1+";\n"
    +"var water2 = "+water2+";\n"
    +"var water3 = "+water3+";\n"
    +"var water4 = "+water4+";\n"
    +"var water5 = "+water5+";\n"
    +"var water6 = "+water6+";\n"
    +"</script>";
    int bytesWritten = file.print(settings);
    file.close();
    if (bytesWritten>0)
      return true;
    else
      return false;
  }
  bool loadSettings() {
    String path = root+"settings.js";
    File file = SPIFFS.open(path, "r");
    if (!file) {
      Serial.println("Error opening file for reading");
      return false;
    }
    String openTag = file.readStringUntil('\n');
    String buf;
    String interval = file.readStringUntil('\n');
    buf = getValue(interval,'=',1);
    buf.replace(";", "");
    intervalWatering = strToIntervalWatering(buf);
    String power = file.readStringUntil('\n');
    buf = getValue(power,'=',1);
    buf.replace(";", "");
    pumpPwm = strToPumpPwm(buf);
    String water1 = file.readStringUntil('\n');
    buf = getValue(water1,'=',1);
    buf.replace(";", "");
    wateringTime[0] = strToWateringTime(buf);
    String water2 = file.readStringUntil('\n');
    buf = getValue(water2,'=',1);
    buf.replace(";", "");
    wateringTime[1] = strToWateringTime(buf);
    String water3 = file.readStringUntil('\n');
    buf = getValue(water3,'=',1);
    buf.replace(";", "");
    wateringTime[2] = strToWateringTime(buf);
    String water4 = file.readStringUntil('\n');
    buf = getValue(water4,'=',1);
    buf.replace(";", "");
    wateringTime[3] = strToWateringTime(buf);
    String water5 = file.readStringUntil('\n');
    buf = getValue(water5,'=',1);
    buf.replace(";", "");
    wateringTime[4] = strToWateringTime(buf);
    String water6 = file.readStringUntil('\n');
    buf = getValue(water6,'=',1);
    buf.replace(";", "");
    wateringTime[5] = strToWateringTime(buf);
    String closeTag = file.readStringUntil('\n');
    settings = openTag+'\n'
    +interval+'\n'
    +power+'\n'
    +water1+'\n'
    +water2+'\n'
    +water3+'\n'
    +water4+'\n'
    +water5+'\n'
    +water6+'\n'
    +closeTag;
    file.close();
    if (settings.length()>0)
      return true;
    else
      return false;
  }

  String getValue(String data, char separator, int index)
  {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
      if(data.charAt(i)==separator || i==maxIndex){
          found++;
          strIndex[0] = strIndex[1]+1;
          strIndex[1] = (i == maxIndex) ? i+1 : i;
      }
    }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
  }

  const String loadWebPageChunk(const String& filename) {
    String path = root+filename;
    File file = SPIFFS.open(path, "r");
     if (!file) {
      Serial.println("Error opening file for reading");
      return "";
    }
    String data = file.readString();
    file.close();
    return data;
  }

  String root;
  String settings;
  String script;
  String html;

  unsigned long intervalWatering = 30;
  int pumpPwm = 512;
  long wateringTime[6] = {1000, 1000, 1000, 1000, 1000, 1000};
  bool isWateringNowSet = false;
  FtpServer ftp;
};
#endif
