/*
Example sketch for interfacing with the DS1302 timekeeping chip.

Copyright (c) 2009, Matt Sparks
All rights reserved.

http://quadpoint.org/projects/arduino-ds1302
*/
#include <stdio.h>
#include <string.h>
#include "DS1302.h"
#include "seven_digit_led.h"
#include "print_time.h"

/* Set the appropriate digital I/O pin connections */
//uint8_t CE_PIN   = 13; //5; // RST?
//uint8_t IO_PIN   = A1; //6; // DAT?
//uint8_t SCLK_PIN = A0; //7; // CLK


#define NANO_IN_USE
#ifdef NANO_IN_USE
    #define CE_PIN   4  // use D4 for RST
    #define IO_PIN   3  // use D3 for DAT
    #define SCLK_PIN 2  // use D2 for CLK
#endif

// #define UNO_IN_USE
#ifdef UNO_IN_USE
    #define CE_PIN   13  // RST?
    #define IO_PIN   12  // DAT?
    #define SCLK_PIN 11  // CLK
#endif

//#define CE_PIN   7  // RST?
//#define IO_PIN   6  // DAT?
//#define SCLK_PIN 5  // CLK

// const unsigned int LED_PIN = A0;

/* Create buffers */
char buf[50];
char day[10];

char dateString[30];
seven_digit_led *sdl;
const int DIGIT_DELAY = 2000; // 2ms optimal
const int NUM_DIGITS = 1000;
long nextChange;
long oneSecond;

/* Create a DS1302 object */
DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN);

void setup()
{
  Serial.begin(9600);
  sdl = new seven_digit_led(4);
  rtc.halt(false);

// #define SET_TIME
#ifdef SET_TIME
  /* Initialize a new chip by turning off write protection and clearing the
     clock halt flag. These methods needn't always be called. See the DS1302
     datasheet for details. */
  rtc.write_protect(false);

  /* Make a new time object to set the date and time */
  //Time t(2013, 10, 31, 16, 13, 00, 3);
  Time t(2015, 10, 13, 14, 17, 0, 3);

  /* Set the time and date on the chip */
  rtc.time(t);
#endif
}


/* Loop and print the time every second */
void loop()
{
    long time = millis();
    if (time >= nextChange)
    {
        // currentDigit = (millis()/1000) % NUM_DIGITS;
        // sdl->show_number(currentDigit);
        nextChange = time + DIGIT_DELAY;

        /* Get the current time and date from the chip */
        Time t = rtc.time();
        //int _hour_to_print = t.hr; //hour();
        //int _min_to_print = t.min; //min();
        //int _sec_to_print = t.sec; //sec();
        //Serial.println(_sec_to_print);
        print_time(t);
        // sdl->show_number(_hour_to_print * 100 + _min_to_print);
        // sdl->show_number(_min_to_print * 100 + _sec_to_print);
    }

  //long time = millis();
  if (time >= oneSecond)
  {
      oneSecond = time + DIGIT_DELAY;
      // currentDigit = (millis()/1000) % NUM_DIGITS;
      // sdl->show_number(currentDigit);
      //oneSecond = time + NUM_DIGITS;
      // print_time();
    }
}

void print_time(Time t)
{
  // Get the current time and date from the chip
  // t = rtc.time();
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
