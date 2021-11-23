/*
Now we need a LedControl to work with.
***** These pin numbers will probably not work with your hardware *****
pin 12 is connected to the DataIn
pin 11 is connected to the CLK
pin 10 is connected to LOAD
We have only a single MAX72XX.
*/
// LedControl lc=LedControl(12, 11, 10, 1);

#include "LedControl.h"
#include "arrayInvader.h"
 
/*
Now we need a LedControl to work with.
***** These pin numbers will probably not work with your hardware *****
pin 12 is connected to the DataIn, DIN
pin 11 is connected to the CLK
pin 10 is connected to LOAD
We have only a single MAX72XX.
*/

#define ARDUINO_IN_USE 
#ifdef ARDUINO_IN_USE 
    #define DATA_IN_PIN 12
    #define CLK_PIN 11
    #define LOAD_PIN 10
#endif
/*
 Arduino NANO, pin #XX 
 */
// #define NANO_IN_USE 
#ifdef NANO_IN_USE 
    #define DATA_IN_PIN 5  // (D5)
    #define CLK_PIN     12 // (D9)
    #define LOAD_PIN    13 // (D13)
#endif

// const int numDevices = 4;      // number of MAX7219s used
const int numDevices = 1;      // number of MAX7219s used
const long scrollDelay = 75;   // adjust scrolling speed 
unsigned long delayTime=200;  // Delay between Frames

void sinvader1a();
void sinvader1b();
void sinvader2a();
void sinvader2b();

LedControl lc=LedControl(DATA_IN_PIN, CLK_PIN, LOAD_PIN, numDevices);
// LedControl lc=LedControl(12, 11, 10, 2);  // Pins: DIN,CLK,CS, number of Display connected
 
void setup()
{
    for (int x=0; x<numDevices; x++)
    {
        lc.shutdown(x, false);      //The MAX72XX is in power-saving mode on startup
        lc.setIntensity(x, 4);      // Set the brightness to default value
        lc.clearDisplay(x);         // and clear the display
    }
    Serial.begin(9600);
    Serial.println("App started  >------------------>");
#ifdef ARDUINO_IN_USE 
    Serial.println("Arduino UNO");
#endif
#ifdef NANO_IN_USE 
    Serial.println("Arduino NANO");
#endif
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
