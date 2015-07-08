/*
Now we need a LedControl to work with.
***** These pin numbers will probably not work with your hardware *****
pin 12 is connected to the DataIn
pin 11 is connected to the CLK
pin 10 is connected to LOAD
We have only a single MAX72XX.
*/
// LedControl lc=LedControl(12, 11, 10, 1);

#define DEVICE 0

#include "LedControl.h"
 
LedControl lc=LedControl(12, 11, 10, 2);  // Pins: DIN,CLK,CS, number of Display connected

const int numDevices = 4;      // number of MAX7219s used
const long scrollDelay = 75;   // adjust scrolling speed 
unsigned long delayTime=200;  // Delay between Frames
 
// Put values in arrays
byte invader1a[] =
{
   B00011000,  // First frame of invader #1
   B00111100,
   B01111110,
   B11011011,
   B11111111,
   B00100100,
   B01011010,
   B10100101
};
 
byte invader1b[] =
{
  B00011000, // Second frame of invader #1
  B00111100,
  B01111110,
  B11011011,
  B11111111,
  B00100100,
  B01011010,
  B01000010
};
 
byte invader2a[] =
{
  B00100100, // First frame of invader #2
  B00100100,
  B01111110,
  B11011011,
  B11111111,
  B11111111,
  B10100101,
  B00100100
};
 
byte invader2b[] =
{
  B00100100, // Second frame of invader #2
  B10100101,
  B11111111,
  B11011011,
  B11111111,
  B01111110,
  B00100100,
  B01000010
};
 
void setup()
{
    for (int x=0; x<numDevices; x++)
    {
        lc.shutdown(x, false);      //The MAX72XX is in power-saving mode on startup
        lc.setIntensity(x, 4);      // Set the brightness to default value
        lc.clearDisplay(x);         // and clear the display
    }
}
 
 
//  Take values in Arrays and Display them
void sinvader1a()
{
  for (int i = 0; i < 8; i++)  
  {
    lc.setRow(0, i, invader1a[i]);
  }
}
 
void sinvader1b()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, invader1b[i]);
  }
}
 
void sinvader2a()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(1, i, invader2a[i]);
  }
}
 
void sinvader2b()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(1, i, invader2b[i]);
  }
}
 
void loop()
{
// Put #1 frame on both Display
    sinvader1a();
    delay(delayTime);
    sinvader2a();
    delay(delayTime);
  
// Put #2 frame on both Display
    sinvader1b();
    delay(delayTime);
    sinvader2b();
    delay(delayTime); 
}