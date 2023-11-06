/*
 *
 */

#include "RTOS_arduino.h"

const unsigned int LED_PIN = A0;
// const unsigned int LED_PIN = 13; // Internal LED


int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.

long interval = 500; // interval to switch, in milliseconds

void setup()
{
    // set the digital pin as output:
    pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    timersWork();

    if (0)//  (cycle_1s)
    {
        if (ledState == LOW)
            ledState = HIGH;
        else
            ledState = LOW;

        // set the LED with the ledState of the variable:
        digitalWrite(LED_PIN, ledState);
    }

    unsigned long currentMillis = millis();

    if(currentMillis - previousMillis > interval)
    {
        previousMillis = currentMillis;

        if (ledState == LOW)
            ledState = HIGH;
        else
            ledState = LOW;

        // set the LED with the ledState of the variable:
        digitalWrite(LED_PIN, ledState);
    }
    eraseCycles();
}
