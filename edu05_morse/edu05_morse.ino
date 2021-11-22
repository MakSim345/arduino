/*
  Blink Morse code
  Turns on an LED in SOS mode, repeatedly.
 */
 
#include "morse.h"
#define LED_PIN 13
#define LED_PIN_GREEN 12

void setup()
{
  // pinMode(LED_PIN, OUTPUT);
}

// create an instance of the Morse class:
Morse morse(LED_PIN);
Morse morse2(LED_PIN_GREEN);

void send_three_dots(Morse _mrs)
{
  _mrs.dot(); 
  _mrs.dot(); 
  _mrs.dot();
}

void send_three_dash(Morse _mrs)
{
  _mrs.dash(); 
  _mrs.dash(); 
  _mrs.dash();
}

void loop()
{
  send_three_dots(morse);
  send_three_dash(morse);
  send_three_dots(morse);
  
  delay(1000);
  
  send_three_dots(morse2);
  send_three_dash(morse2);
  send_three_dots(morse2);
  
  delay(1000);
}

