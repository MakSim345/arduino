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
int potPin = 5; //input read pin for LM35 is Analog Pin 5 (A5 on the board, not just 5!)
int temperature = 0;  //variable which will be calculated in process
long val=0; //variable to store the value coming from the sensor
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
    val = analogRead(potPin); //read the value of sensor
    temperature = (5*val*100/1024); //convert voltage to temperature
    Serial.println("\n---Current temperature: ");
    Serial.println ((float)temperature);
    //print temperature value on serial screen
    //lcd.setCursor(0,1);
    //lcd.print((float)temperature);
    //lcd.setCursor(3,1);
    //lcd.print("D.Centigrade");
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    // set the LED with the ledState of the variable:
    digitalWrite(LED_PIN, ledState);

    delay(1000);  //wait for 2seconds
} //End of process, go back to start of loop - ie check temp...
 
