#include <Bounce.h>

#define BUTTON 2
#define LED 8
#define TIME_CHATTER 5

//create Bounce object.
//Set pin for button and time for bounce in milliseconds:
Bounce bouncer = Bounce(BUTTON, TIME_CHATTER);

//set LED to ON initially:
int ledValue = LOW;

void setup()
{
  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);
}

void loop()
{
  //if button change state:
  if ( bouncer.update() )
  {
    //if 1 read:
    if ( bouncer.read() == HIGH)
    {
     //reverce LED:
     if ( ledValue == LOW )
     {
       ledValue = HIGH;
     }
     else
     {
       ledValue = LOW;
     }
     digitalWrite(LED, ledValue);
    }
  }
}

