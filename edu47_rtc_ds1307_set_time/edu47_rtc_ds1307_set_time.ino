#include <Wire.h>
#include "RTClib.h"
#include <Time.h>

RTC_DS1307 RTC;

/*
For RTC DS1307 in NANO:
SCL is pin A5 and SDA is pin A4 on the Nano
SCL = A5
SDL = A4
*/

void setup ()
{
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();

    if (! RTC.isrunning())
    {
      Serial.println("RTC is NOT running!");
      // following line sets the RTC to the date & time this sketch was compiled
      RTC.adjust(DateTime(__DATE__, __TIME__));
  }
      //RTC.adjust(DateTime(__DATE__, __TIME__));
}


void loop ()
{

    DateTime now = RTC.now();

    Serial.print(now.day());
    Serial.print('-');
    Serial.print(now.month());
    Serial.print('-');
    Serial.println(now.year());

    Serial.print(now.hour());
    Serial.print(':');
    Serial.print(now.minute());
    Serial.print(':');
    Serial.println(now.second());

   /*int day_of_week = now.dayOfWeek();
   switch(day_of_week)
   {
     case 1:
      Serial.println("Monday");
      break;
     case 2:
      Serial.println("Tuesday");
      break;
     case 3:
      Serial.println("Wednesday");
      break;
     case 4:
      Serial.println("Thursday");
      break;
     case 5:
      Serial.println("Friday");
      break;
     case 6:
      Serial.println("Saturday");
      break;
     case 0:
      Serial.println("Sunday");
      break;
    }
    */
    delay(1000);
    /*
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
      // RTC.adjust(curTime + (millis()-eps)/1000);
    }
    */
}
