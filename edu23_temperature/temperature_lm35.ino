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
int potPin = 5; //input read pin for LM35 is Analog Pin 5
int temperature = 0;  //variable which will be calculated in process
long val=0; //variable to store the value coming from the sensor
// LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
 
void setup()
{ 
    //lcd.begin(16, 2);
    Serial.begin(9600);
    Serial.println("LM35 Thermometer ");
    //lcd.print("Thermometer ");
}
 
void loop ()  //loop below process
{
    val = analogRead(potPin); //read the value of sensor
    temperature = (5*val*100/1024); //convert voltage to temperature
    Serial.println ((float)temperature);
    //print temperature value on serial screen
    //lcd.setCursor(0,1);
    //lcd.print((float)temperature);
    //lcd.setCursor(3,1);
    //lcd.print("D.Centigrade");
    Serial.println("D.Centigrade");
    delay(2000);  //wait for 2seconds
} //End of process, go back to start of loop - ie check temp...
 
