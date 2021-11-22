/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
  This example code is in the public domain.
 */
 
#include "CycleTiming.h"

#define LED_PIN 13

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int ledPin = 13;
int ledGreenPin = 12;
int longWait = 500;
int shortWait = 200;
long randOn = 0;                  // Initialize a variable for the ON time
long randOff = 0;                 // Initialize a variable for the OFF time

void random_leds();

boolean ledState = false;

// instant class:
CycleTiming timer(250);

// the setup routine runs once when you press reset:
void setup() 
{
  pinMode(LED_PIN, OUTPUT);
  // initialize the digital pin as an output.
  //pinMode(ledPin, OUTPUT); 
  pinMode(ledGreenPin, OUTPUT); 
  randomSeed (analogRead (0));    // randomize 
  Serial.begin(9600);   
}


// the loop routine runs over and over again forever:
void loop() 
{
    static int nFlag = 0;
    if (0 == nFlag)
    {
        Serial.write("\n\nHello. App start!\n\n");
        nFlag = 1;
    }

    if (timer.check()) 
    {
        ledState = !ledState;
        // digitalWrite(LED_PIN, ledState);
        random_leds();
    }
}

void random_leds()
{
  
  randOn = random (100, 1200);    // generate ON time between 0.1 and 1.2 seconds
  randOff = random (200, 900);    // generate OFF time between 0.2 and 0.9 seconds
  
  Serial.write("--\n"); // 
  char v_str[8] = "       ";  //reserve the string space first
  itoa(randOn, v_str, 6);
  // lcd.printIn(v_str);
  int bytesSent = Serial.write(v_str); //send the string and return the length of the string.
  Serial.write("--\n"); 

  digitalWrite(ledPin, HIGH);   // sets the LED on
  digitalWrite(ledGreenPin, LOW);
  delay(randOn);                // waits for a random time while ON
  
  digitalWrite(ledPin, LOW);    // sets the LED off
  digitalWrite(ledGreenPin, HIGH);
  delay(randOff);               // waits for a random time while OFF
}
