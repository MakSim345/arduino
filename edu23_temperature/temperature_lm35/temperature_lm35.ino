/*
* LM35 Thermometer
*
* LM35 simpy connected to: 
* 5+
* 0V
* Analog Pin 5
* www.circuit-blog.com
* See more at: http://www.buildcircuit.com/heat-sensor-using-lm35-and-arduino/#sthash.4yEcLbZx.dpuf
*/
 
// #include <LiquidCrystal.h>
int POT_PIN = 0; //input read pin for LM35 is Analog Pin 5 (A5 on the board, not just 5!)
// #define POT_PIN 0 //input read pin for LM35 is Analog Pin X (AX on the board, not just X!)
int temperature = 0;  //variable which will be calculated in process
long reading=0; //variable to store the value coming from the sensor
// LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
const unsigned int LED_PIN =  13;
// Variables will change:
int ledState = LOW;             // ledState used to set the LED 

void setup()
{ 
    //lcd.begin(16, 2);
    Serial.begin(9600);
    Serial.println("LM35 Thermometer - Start.");
    //lcd.print("Thermometer ");
    // set the digital pin as output:
    pinMode(LED_PIN, OUTPUT);

}
 
void loop ()  //loop below process
{
    
    reading = analogRead(POT_PIN); //read the value of sensor
    temperature = (5*reading*100/1024); //convert voltage to temperature
    Serial.println("\n---Current temperature: ");
    Serial.println ((float)temperature);
    
    float voltage = (reading * 5.0) / 1024.0; 
     
    // print out the voltage
    Serial.print(voltage); 
    Serial.println(" volts");

    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    // set the LED with the ledState of the variable:
    digitalWrite(LED_PIN, ledState);

 /*
    //getting the voltage reading from the temperature sensor
    int reading = analogRead(POT_PIN);  
 
    // converting that reading to voltage, for 3.3v arduino use 3.3
    float voltage = reading * 5.0;
    voltage /= 1024.0; 
     
    // print out the voltage
    Serial.print(voltage); 
    Serial.println(" volts");
 
    // now print out the temperature
    float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                                //to degrees ((voltage - 500mV) times 100)
    Serial.print(temperatureC); 
    Serial.println(" degrees C");
     
    // now convert to Fahrenheit
    float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
    Serial.print(temperatureF); Serial.println(" degrees F");
   */  
    delay(1000);                                     //waiting a second
    
} //End of process, go back to start of loop - ie check temp...
 
