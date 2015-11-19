#include <Wire.h>
#include "RTClib.h"
#include <Time.h>

RTC_DS1307 RTC;

void setup () 
{
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();
}

void loop () 
{
    time_t curTime = 0;
    // to Serial - unixtime
    if (Serial.available() > 0)
    {
      int eps = millis();
      time_t coef = 1000000000;
      while (coef > 0)
      {
         byte b = Serial.read();
         if (b == 255)
         {
            continue; 
         }
         curTime += coef*(b-'0');
         coef /= 10;
      }
      // Set time RTC
      RTC.adjust(curTime + (millis()-eps)/1000);
    }
}