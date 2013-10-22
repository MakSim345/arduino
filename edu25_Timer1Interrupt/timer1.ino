#include "TimerOne.h"

// pin 13 has an LED connected on most Arduino boards.
int led = 13;
volatile int ledState = LOW;

void setup()
{
  pinMode(led, OUTPUT);
  Timer1.initialize(1000000);         // initialize timer1. unit: microseconds
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
}
 
void callback()
{
  ledState = !ledState;
}
 
void loop()
{
  digitalWrite(led, ledState);
}
