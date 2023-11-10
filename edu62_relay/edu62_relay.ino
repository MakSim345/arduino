/*
 *
 */

#include "RTOS_arduino.h"

// const unsigned int RELAY_PIN = A0;
const unsigned int RELAY_PIN = 13; // Internal LED

int gpioState = LOW;            // to set the relay ON/OFF
long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.

// long interval = 1000; // interval to tick, in milliseconds
// long interval = 100; // interval to tick, in milliseconds

const long INTERVAL_100_MS  = 100;
const long INTERVAL_500_MS  = 500;
const long INTERVAL_1000_MS = 1000;

bool isSwitchOn = false;

const unsigned int mainCounterValue = 5; // relay ON in seconds

unsigned int calculatedCounterValue = mainCounterValue * 10; // relay ON in (100 ms * 10)
// unsigned int calculatedCounterValue = mainCounterValue * 2;  // relay ON in (500 ms * 2)
unsigned int currentCounterValue = 0;

void setup()
{
    Serial.begin(9600);
    // set the digital pin as output:
    pinMode(RELAY_PIN, OUTPUT);
    currentCounterValue = calculatedCounterValue;
    isSwitchOn = false;
    gpioState = LOW;
}

void loop()
{
    unsigned long currentMillis = millis();

    if(currentMillis - previousMillis > INTERVAL_100_MS)
    {
        // Serial.println("Main loop - 100 ms");
        previousMillis = currentMillis;
        if (isSwitchOn)
        {
          Serial.println("Main loop - Switch is On");

          if (gpioState == LOW)
          {
            Serial.print("Swich ON in: ======");
            Serial.println(millis());
            gpioState = HIGH;
            digitalWrite(RELAY_PIN, gpioState);
          }

          isSwitchOn = false;
          // reset counter back to origin:
          currentCounterValue = calculatedCounterValue;
          Serial.print("Set to OFF in: ====");
          Serial.println(millis());

          //Serial.print("currentCounterValue: ");
          //Serial.println(currentCounterValue);
        }
        else // isSwitchOn = false;
        {
            // Serial.println("Main loop - Switch is OFF");
            //Serial.print("currentCounterValue: ");
            //Serial.println(currentCounterValue);

            if (gpioState == HIGH)
            {
                Serial.print("Swich OFF in: ======");
                Serial.println(millis());
                gpioState = LOW;
                digitalWrite(RELAY_PIN, gpioState);
            }

            currentCounterValue--;
            if (currentCounterValue <= 0)
            {
              isSwitchOn = true;
              Serial.print("Counter Zero in: ==");
              Serial.println(millis());
            }
        }
     }
}
