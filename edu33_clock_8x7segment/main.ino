/*
A clock without RTC, using 8 seven-segment leds.
 */

#include "segment8x7.h"
#include "Time.h" 

long nextChange;
int currentDigit = -1;
const int DIGIT_DELAY = 5; // 2ms optimal
const int NUM_DIGITS = 1000;
char dateString[30];
segment8x7 *sdl;

void setup() 
{
    sdl = new segment8x7();
    nextChange = 0;
    Serial.begin(9600);
    setTime(1, 33, 0, 29, 12, 2013); // HH-MM-SS DD-MM-YYYY
}

void loop() 
{
    long time = millis();
    if (time >= nextChange) 
    {
        currentDigit = (millis()/1000) % NUM_DIGITS;
        // sdl->show_number(currentDigit);
        nextChange = time + DIGIT_DELAY;
    
/*
  if(lowerSeconds%2)
  {
    sprintf(dateString," Time: %d%d %d%d %d%d",upperHours,lowerHours,upperMins,lowerMins,upperSeconds,lowerSeconds);
  }
  else
  {
    sprintf(dateString," Time: %d%d:%d%d:%d%d",upperHours,lowerHours,upperMins,lowerMins,upperSeconds,lowerSeconds);
  }
*/ // digital clock display of the time
    int _hour_to_print = hour();
    int _min_to_print = minute();
    int _sec_to_print = second();
 
    // sdl->show_number(_hour_to_print * 10000 + _min_to_print * 100 + _sec_to_print);
    // sdl->show_number(_hour_to_print * 100 + _min_to_print);
    
    sdl->show_hour(_hour_to_print);
    sdl->show_min (_min_to_print);
    sdl->show_sec (_sec_to_print);
    // sdl->show_number(245813);
    
    //sdl->show_number(nextChange);
    //nextChange++;
  /*
  Serial.print(hour());
  Serial.print(":");

  Serial.print(minute());
  Serial.print(":");
  
  Serial.print(second());
  //printDigits(minute());
  //printDigits(second());
  Serial.print(" --  ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println();
    //Serial.print(dateString);
    Serial.print("\n");
    //delay(100);
    */
    }
}
