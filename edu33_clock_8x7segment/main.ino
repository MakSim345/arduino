/*
A clock without RTC, using 8 seven-segment leds.
 */

#include <stdio.h>
#include <string.h>
#include "segment8x7.h"
#include "Time.h" 
#include <DS1302.h>

/* Set the appropriate digital I/O pin connections */
uint8_t CE_PIN   = 13; //5; // RST?
uint8_t IO_PIN   = 6;//A1; //6; // DAT?
uint8_t SCLK_PIN = 7;//A0; //7; // CLK


long nextChange;
long oneSecond;
int currentDigit = -1;
const int DIGIT_DELAY = 10; // 2ms optimal
const int ONE_SECOND = 1000;
char dateString[30];
segment8x7 *sdl;

/* Create a DS1302 object */
DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN);

void setTimeInRTC();

int _hour_to_print;
int _min_to_print; 
int _sec_to_print; 

Time t;

void setup() 
{
    sdl = new segment8x7();    
    // Serial.begin(9600);

    // Follow is fot Time.h:
    // setTime(9, 21, 45, 29, 12, 2013); // HH-MM-SS DD-MM-YYYY

    /* Initialize a new chip by turning off write protection and clearing the
     clock halt flag. These methods needn't always be called. See the DS1302
     datasheet for details. */
    rtc.write_protect(false);
    rtc.halt(false);

    // Uncomment this function if new time has to be set to RTC:
    // setTimeInRTC();
}

void loop() 
{
  long time = millis();
  
  if (time >= oneSecond) 
    {
      // Update current time every second.
      // Get the current time and date from the chip:
      t = rtc.time();
      _hour_to_print = t.hr;
      _min_to_print = t.min;
      _sec_to_print = t.sec;
      oneSecond = time + ONE_SECOND;
    }

    if (time >= nextChange) 
    {
        nextChange = time + DIGIT_DELAY;
    
      // digital clock display of the time:
      //int _hour_to_print = hour();
      //int _min_to_print = minute();
      //int _sec_to_print = second();

      sdl->show_hour(_hour_to_print);
      sdl->show_min (_min_to_print);
      sdl->show_sec (_sec_to_print);
    
      // sdl->show_number(245813);    
      //sdl->show_number(nextChange);
      //nextChange++;
    }
}

void setTimeInRTC()
{
    /* Make a new time object to set the date and time */
    /*   Tuesday, May 19, 2009 at 21:16:37.            */
    Time new_time(2013, 12, 30, 11, 8, 45, 3); // YYYYMM DD HH-MM-SS

    /* Set the time and date on the chip */
    rtc.time(new_time);

}
