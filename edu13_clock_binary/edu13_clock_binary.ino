#include "Arduino.h"
#include "Charlieplex.h"


/*
* source:
* http://gliffer.ru/articles/diy-ili-sdelay-sam--chasi-na-30-svetodiodah/
*/

unsigned long lastTick = 0, frameTick = 0,  setTick = 0, cur_millis;// 
int hour=0, minute=0, second=0; //start the time on 00:00:00
int munit, ihour, idecmin, iunitmin;
int frameNum = 0;
boolean ch =  true, sets = false; // 

#define NUMBER_OF_PINS 6
//define pins in the order you want to adress them
byte pins[] = {7,8,9,10,11,12};

#define PIN_SET_HOURS 5 // button for set hours
#define PIN_SET_MINS 6 // button for set minutes

//initialize object for the Charlieplexing
Charlieplex charlieplex = Charlieplex(pins, NUMBER_OF_PINS);

// pins in pairs for the hours 
charliePin leds_hours[12] = 
{ 
  {4, 5}, {4, 3}, {5, 3}, {0, 4}, {1, 4}, {2, 4}, {3, 4}, {5, 4}, {0, 5}, {1, 5} , {2, 5}, {3, 5}
};

// pins in pairs for the decades of minutes
charliePin leds_decmin[5] = 
{
  {2, 3}, {1, 3}, {0, 3}, {5, 2}, {4, 2}
};

// pins in pairs for the minutes
charliePin leds_min[9] = 
{
  {3, 2}, {1, 2}, {0, 2}, {5, 1}, {4, 1}, {3, 1}, {2, 1}, {0, 1}, {5,0} 
};

// pins in pairs for the alarms
charliePin leds_alarm[4] = 
{
  {1, 0}, {2, 0}, {3, 0}, {4, 0} 
};


void setup()
{  
  ch = true;
  lastTick = 0;
  setup_sets;
}

void loop()
{
  
  cur_millis  = millis(); // current time count
  
  if (cur_millis - lastTick >= 1000) 
  {
    lastTick = cur_millis;
    second++;
  }

  // move forward one minute every 60 seconds
  if (second >= 60) 
  {
    minute++;
    second = 0; // reset seconds to zero
    ch = true;  // flag of change in status of leds
  }
  
  // move forward one hour every 60 minutes
  if (minute >=60) 
  {
    hour++;
    minute = 0; // reset minutes to zero
  }
  
  if (hour >=12) 
  {
    hour=0;
    minute = 0; // reset minutes to zero
  }
   
  if (ch) 
  { // flag of change in status of leds
    ihour = hour;
    munit = minute%10;
    iunitmin = munit-1;
    idecmin = (minute - munit) / 10 - 1;
    ch = false;
  }

  if (cur_millis - frameTick >= 1) 
  { 
  // shift of frame
    frameTick = cur_millis;
    frameNum++;
    if (frameNum > 2) {
      frameNum = 0;
  }
  
  charlieplex.clear(); // clear all
    
    switch (frameNum) 
    {
      case 0: // show 0 frame
        showpin(leds_hours[ihour]); 
        break;
      case 1: // show 1 frame
        if (idecmin >=0) 
        {
          showpin(leds_decmin[idecmin]);
        }
        break;
      case 2: // show 2 frame
        if (iunitmin >=0 ) 
        {
          showpin(leds_min[iunitmin]);
        }
        break;
    }
  }
   
  check_sets();
  
}

void showpin(charliePin pin) 
{
  charlieplex.charlieWrite(pin, HIGH); 
}

void setup_sets(void) 
{
  pinMode(PIN_SET_HOURS, INPUT);     
  pinMode(PIN_SET_MINS, INPUT);     
}

void check_sets(void) 
{ // check status of the buttons
  if (sets) 
  { 
    // if in status of set then delay
    if (cur_millis - setTick >= 300) 
    {
       sets = false;
       setTick = 0;
    } 
  } 
  else 
  {
    if (digitalRead(PIN_SET_HOURS) == HIGH) 
    {
      hour++; 
      sets = true; 
      ch = true; 
      setTick = cur_millis;
    }
    if (digitalRead(PIN_SET_MINS) == HIGH) 
    {
      minute++; 
      sets = true; 
      ch = true; 
      setTick = cur_millis;
    }
  }  
}
