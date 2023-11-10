/*
  Blink Morse code
  Turns on an LED in SOS mode, repeatedly.
 */
 
#include "morse.h"
#define LED_PIN 13
#define LED_PIN_GREEN 12

void setup()
{
  Serial.begin(9600);
}

int startDelay = 500;  // Example startDelay value

void loop() {
  // Calculate the formula
  int CurrentRR = 60.0 /( (startDelay * 2) / 1000.0);

  int TargetRR = 150;
  int delayRR = 60000/(2.0 * TargetRR);

  // Send the result to the serial port
  Serial.print("CurrentRR: ");
  Serial.println(CurrentRR);

  Serial.print("delayRR: ");
  Serial.println(delayRR);
  delay (3000);

  // Continue with other tasks in the loop
  // ...
}
