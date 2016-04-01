/*
 counter for testing LED 7 segments display code.
 */

#include "seven_digit_led.h"

// comment it out if LM35 is in use!
#define TM36_IN_USE

long led_update = 0;
long tmp_update = 0; // temperature
int currentDigit = -1;
const int DIGIT_DELAY = 8; // led update
const int TEMP_DELAY = 10000; // temp update: 10 sec.
const int NUM_DIGITS = 1000;

seven_digit_led *sdl;
int temperature = 0;  //variable which will be calculated in process
long sensorVal=0; //variable to store the value coming from the sensor
int POT_PIN = 0; //input read pin for LM35 is Analog Pin X (AX on the board, not just X!)

void setup() 
{
    sdl = new seven_digit_led(4);
    // Turn the Serial Protocol ON
    Serial.begin(9600);
#ifdef TM36_IN_USE
    Serial.println("TM36 Thermometer - Start.");
#else
    Serial.println("LM35 Thermometer - Start.");
#endif    
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
        sensorVal = analogRead(POT_PIN); //read the value of sensor
        float voltage = (sensorVal/1024.0) * 5.0;
#ifdef TM36_IN_USE
        float temperature = (voltage - .5) * 100;
        Serial.print("Sensor TM36 temperature: ");
#else                
        // Voltage at pin in milliVolts = (reading from ADC) * (5000/1024) 
        temperature = (sensorVal)*(500/1024.0); //convert voltage to temperature
        Serial.print("Sensor LM35 temperature: ");
#endif        
        Serial.print((float)temperature);
        Serial.print("\n");

        currentDigit = temperature;// (millis()/1000) % NUM_DIGITS;
        //sdl->show_number(1114);
        tmp_update = time + TEMP_DELAY;
    }
}


