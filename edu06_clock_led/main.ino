/*
 counter for testing LED 7 segments display code.
 */

#include "seven_digit_led.h"

long nextChange;
int currentDigit = -1;
const int DIGIT_DELAY = 5; // 2ms optimal
const int NUM_DIGITS = 1000;

seven_digit_led *sdl;

void setup() 
{
    sdl = new seven_digit_led(4);
    nextChange = 0;
    Serial.begin(9600);
}

void loop() 
{
    long time = millis();
    if (time >= nextChange) 
    {
        currentDigit = (millis()/1000) % NUM_DIGITS;
        sdl->show_number(currentDigit);
        nextChange = time + DIGIT_DELAY;
    }
    //sdl->show_number(1114);
}


