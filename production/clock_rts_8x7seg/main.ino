/*
A clock with RTC module, using 8 seven-segment leds.
 */

#include <stdio.h>
#include <string.h>
#include <DS1302.h>
#include "seven_digit_led.h"
#include "print_time.h"
#include "segment8x7.h"

/* Set the appropriate digital I/O pin connections */
uint8_t CE_PIN   = 13; //5; // RST?
uint8_t IO_PIN   = A1; //6; // DAT?
uint8_t SCLK_PIN = A0; //7; // CLK

// const unsigned int LED_PIN = A0;

/* Create buffers */
char buf[50];
char day[10];

char dateString[30];
const int DIGIT_DELAY = 5; // 2ms optimal
const int NUM_DIGITS = 1000;
long nextChange;
long oneSecond;

// Create a DS1302 object
DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN);

// create a display object:
segment8x7 *sdl;

void setTimeInRTC();

void setup()
{
  Serial.begin(9600);
  sdl = new segment8x7();    

  /* Initialize a new chip by turning off write protection and clearing the
     clock halt flag. These methods needn't always be called. See the DS1302
     datasheet for details. */
  rtc.write_protect(false);
  rtc.halt(false);
  
  // Uncomment this function if new time has to be set to RTC:
  // setTimeInRTC();  
}


/* Loop and print the time every second */
void loop()
{
    long time = millis();
    if (time >= nextChange) 
    {
        nextChange = time + DIGIT_DELAY;
        //print_time();
        /* Get the current time and date from the chip */
        Time t = rtc.time();
        int _hour_to_print = t.hr; //hour();
        int _min_to_print = t.min; //min(); 
        int _sec_to_print = t.sec; //sec(); 
              
        sdl->show_hour(_hour_to_print);
        sdl->show_min (_min_to_print);
        sdl->show_sec (_sec_to_print);
    

        // sdl->show_number(_hour_to_print * 100 + _min_to_print);
        // sdl->show_number(_min_to_print * 100 + _sec_to_print);
    }
    
    if (time >= oneSecond) 
    {
        oneSecond = time + NUM_DIGITS;
        print_time();
    }
}

void setTimeInRTC()
{
    
  /* Make a new time object to set the date and time */
  Time t(2014, 2, 27, 8, 50, 00, 3);

  /* Set the time and date on the chip */
  rtc.time(t);
}

void print_time()
{  
  // Get the current time and date from the chip
  Time t = rtc.time();

  // Name the day of the week
  memset(day, 0, sizeof(day));  // clear day buffer
  switch (t.day) 
  {
    case 1:
      strcpy(day, "Sunday");
      break;
    case 2:
      strcpy(day, "Monday");
      break;
    case 3:
      strcpy(day, "Tuesday");
      break;
    case 4:
      strcpy(day, "Wednesday");
      break;
    case 5:
      strcpy(day, "Thursday");
      break;
    case 6:
      strcpy(day, "Friday");
      break;
    case 7:
      strcpy(day, "Saturday");
      break;
  }

  // Format the time and date and insert into the temporary buffer
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
           day,
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);

  // Print the formatted string to serial so we can see the time
  Serial.println(buf); 
}
