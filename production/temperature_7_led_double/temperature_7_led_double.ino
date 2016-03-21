/*
 counter for testing LED 7 segments display code.
 */

#include "seven_digit_led.h"

long led_update = 0;
long tmp_update = 0; // temperature
int currentDigit = -1;
const int DIGIT_DELAY = 8; // led update
const int TEMP_DELAY = 10000; // temp update: 10 sec.
const int NUM_DIGITS = 1000;

seven_digit_led *sdl;
int temperature = 0;  //variable which will be calculated in process
long val=0; //variable to store the value coming from the sensor
int POT_PIN = 0; //input read pin for LM35 is Analog Pin X (AX on the board, not just X!)

void setup() 
{
    sdl = new seven_digit_led(4);
    // Turn the Serial Protocol ON
    Serial.begin(9600);
    Serial.println("LM35 Thermometer - Start.");
}

void loop() 
{
    long time = millis();
    
    if (time >= led_update) 
    {        
        sdl->show_number(currentDigit);
        led_update = time + DIGIT_DELAY;
    }

    if (time >= tmp_update) 
    {
        val = analogRead(POT_PIN); //read the value of sensor
        temperature = (val)*(500/1024.0); //convert voltage to temperature
        // Voltage at pin in milliVolts = (reading from ADC) * (5000/1024) 
        Serial.print("Current temperature: ");
        Serial.print((float)temperature);
        Serial.print("\n");

        currentDigit = temperature;// (millis()/1000) % NUM_DIGITS;
        //sdl->show_number(1114);
        tmp_update = time + TEMP_DELAY;
    }
}


