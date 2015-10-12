//We always have to include the library
#include "LedControl.h"
#include <Time.h> 


// #define NANO_IN_USE 
#define ARDUINO_IN_USE 
/*
Now we need a LedControl to work with.
***** These pin numbers will probably not work with your hardware *****
pin 12 is connected to the DataIn, DIN
pin 11 is connected to the CLK
pin 10 is connected to LOAD
We have only a single MAX72XX.
*/
#ifdef ARDUINO_IN_USE 
    #define DATA_IN_PIN 12
    #define CLK_PIN 11
    #define LOAD_PIN 10
#endif
/*
 Arduino NANO, pin #XX 
 */
#ifdef NANO_IN_USE 
    #define DATA_IN_PIN 5  // (D5)
    #define CLK_PIN     12 // (D9)
    #define LOAD_PIN    13 // (D13)
#endif

LedControl lc=LedControl(DATA_IN_PIN, CLK_PIN, LOAD_PIN, 1);

#define DEVICE 0

char dateString[30];
const int DIGIT_DELAY = 5; // 2ms optimal
const int NUM_DIGITS = 1000;
long nextChan ge;
long oneSecond;
int _sec_to_print = 0;
/* we always wait a bit between updates of the display */
unsigned long delaytime=250;
unsigned long _ctr = 0;

void setup()
{
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  int _device = DEVICE;
  lc.shutdown(_device, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(_device, 10);
  /* and clear the display */
  lc.clearDisplay(_device);

  setTime(21, 23, 0, 3, 7, 2015); // HH-MM-SS DD-MM-YYYY
}


/*
This method will display the characters for the
word "Arduino" one after the other on digit 0.
*/
void writeArduinoOn7Segment()
{
  int _device = DEVICE;
  lc.setChar(_device, 1, 'a', false);
  delay(delaytime);
  lc.setRow(_device, 0, 0x05);
  delay(delaytime);
  lc.setChar(_device, 0, 'd',false);
  delay(delaytime);
  lc.setRow(_device, 0, 0x1c);
  delay(delaytime);
  lc.setRow(_device, 0, B00010000);
  delay(delaytime);
  lc.setRow(_device, 0, 0x15);
  delay(delaytime);
  lc.setRow(_device, 0, 0x1D);
  delay(delaytime);
  lc.clearDisplay(0);
  delay(delaytime);
}

/*
  This method will scroll all the hexa-decimal
numbers and letters on the display. You will need at least
four 7-Segment digits. otherwise it won't really look that good.
*/
void scrollDigits()
{
  int _num = DEVICE;
  for(int i=0; i<13; i++)
  {
    lc.setDigit(_num, 8, i+1,   false);
    lc.setDigit(_num, 7, i+2,   false);
    lc.setDigit(_num, 6, i+3,   false);
    lc.setDigit(_num, 5, i+4,   false);
    lc.setDigit(_num, 4, i+5,   false);
    lc.setDigit(_num, 3, i+6,   false);
    lc.setDigit(_num, 2, i+7, false);
    lc.setDigit(_num, 1, i+8, false);
    lc.setDigit(_num, 0, i+9, false);
    delay(delaytime);
  }

  lc.clearDisplay(0);
  delay(delaytime);
}

void show_sec(int seconds)
{
  byte decimal[8] = {0};
  decimal[1] = seconds / 10;
  decimal[0] = seconds % 10;
 
  lc.setDigit(0, 0, decimal[0], false);
  lc.setDigit(0, 1, decimal[1], false);
}

void show_min(int minutes)
{
  byte decimal[8] = {0};
  decimal[1] = minutes / 10;
  decimal[0] = minutes % 10;
 
  lc.setDigit(0, 3, decimal[0], false);
  lc.setDigit(0, 4, decimal[1], false);
}

void show_hour(int hours)
{
  byte decimal[8] = {0};
  decimal[1] = hours / 10;
  decimal[0] = hours % 10;
 
  lc.setDigit(0, 6, decimal[0], false);
  lc.setDigit(0, 7, decimal[1], false);
}

void displayNumber(unsigned long value)
{
   byte decimal[8] = {0};
   value = value % 100000000;  //ensure the value is within 8 digits only
   decimal[7] = value / 10000000;  //extract digit 7 from value
   value = value % 10000000;       //extract the rest of 7 digit value
   decimal[6] = value / 1000000;
   value = value % 1000000;
   decimal[5] = value / 100000;
   value = value % 100000;
   decimal[4] = value / 10000;
   value = value % 10000;
   decimal[3] = value / 1000;
   value = value % 1000;
   decimal[2] = value / 100;
   value = value % 100;
   decimal[1] = value / 10;
   decimal[0] = value % 10;
   byte zero = 0;
   int _num = 0;

   for(int i=0; i<8; i++)
   {
     lc.setDigit(0, i, decimal[i], false);
   }
}

void loop()
{
  //displayNumber(_ctr);
  //delay(10);
  //_ctr = _ctr + 1;
  //writeArduinoOn7Segment();
  //scrollDigits();
  long time = millis();
  if (time >= nextChange) 
  {
    nextChange = time + DIGIT_DELAY;
    //print_time();
    /* Get the current time and date from the chip */
    //Time t = rtc.time();
    int _hour_to_print = hour();
    int _min_to_print = minute(); 
    _sec_to_print = second(); 
              
    show_hour(_hour_to_print);
    show_min (_min_to_print);
    show_sec (_sec_to_print);
    // sdl->show_number(_hour_to_print * 100 + _min_to_print);
    // sdl->show_number(_min_to_print * 100 + _sec_to_print);
  }
 
}
