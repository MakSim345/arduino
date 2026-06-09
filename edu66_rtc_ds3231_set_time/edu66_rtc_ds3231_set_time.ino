#include <Wire.h>
#include <RTClib.h>
#include <Time.h>

RTC_DS3231 RTC;

void setup ()
{
    Serial.begin(9600);
    // Wire.begin();
    RTC.begin();
    delay(1000);

    if(!RTC.begin())
    {
        Serial.println("No RTC Module found!");
        while(1);
    }

    /*
    if (! RTC.isrunning())
    {
      Serial.println("RTC is NOT running!");
      // following line sets the RTC to the date & time this sketch was compiled
      RTC.adjust(DateTime(__DATE__, __TIME__));
    }
    */
    // RTC.adjust(DateTime(F(__DATE__),F( __TIME__)));
    // RTC.adjust(DateTime(2026, 6, 3, 9, 2, 45));
}


void loop ()
{
    DateTime now = RTC.now();

    Serial.print(now.year(), DEC);
    Serial.print('-');
    if (now.month() < 10)
        Serial.print('0');
    Serial.print(now.month()), DEC;
    Serial.print('-');

    if (now.day() < 10)
        Serial.print('0');
    Serial.print(now.day(), DEC);
    Serial.print(' ');

    if (now.hour() < 10)
        Serial.print('0');
    Serial.print(now.hour());
    Serial.print(':');

    if (now.minute() < 10)
        Serial.print('0');
    Serial.print(now.minute());
    Serial.print(':');

    if (now.second() < 10)
        Serial.print('0');
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

