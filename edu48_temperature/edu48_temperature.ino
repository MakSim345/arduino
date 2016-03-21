// const int sensorPin = A0;
const int sensorPin = 0; //input read pin for LM35 is Analog Pin X (AX on the board, not just X!)

const float baselineTemp = 20.0;
// const int DIGIT_DELAY = 8; // led update
const int TEMP_DELAY = 1500; // temperature update: milliseconds.
long tmp_update = 0; //  tmp value for the loop

void setup()
{
    Serial.begin(9600);

    for(int pinNumber = 2; pinNumber < 5; pinNumber++)
    {
      pinMode(pinNumber, OUTPUT);
      digitalWrite(pinNumber, LOW);
    }
}

void loop()
{
  long time = millis();

  if (time >= tmp_update)
  {
      int sensorVal = analogRead(sensorPin); //read the value of sensor
      // temperature = (val)*(500/1024.0); //convert voltage to temperature
      // Voltage at pin in milliVolts = (reading from ADC) * (5000/1024)

      //Serial.print("Gurigayga Temperaturekiisu waa: ");
      Serial.print("\nRaw value: ");
      Serial.print(sensorVal);
      Serial.print("\n");

      float voltage = (sensorVal/1024.0) * 5.0;
      Serial.print("Volts: ");
      Serial.print(voltage);
      Serial.print("\n");

      float temperature = (voltage - .5) * 100;
      Serial.print("Sensor TM36 temperature: ");
      Serial.print(temperature);
      Serial.print("\n");

      float temper = (sensorVal)*(500/1024.0); //convert voltage to temperature
      // Voltage at pin in milliVolts = (reading from ADC) * (500.0/1024) 
      Serial.print("Sensor LM35 temperature: ");
      Serial.print(temper);
      Serial.print("\n\n");
      

      if(temperature < baselineTemp)
      {
          digitalWrite(2, LOW);
          digitalWrite(3, LOW);
          digitalWrite(4, LOW);
          Serial.print("Guriga dariishadaha iska xir hiitkana shid: ");
      }
      else if(temperature >= baselineTemp + 2 && temperature < baselineTemp + 4)
      {
          digitalWrite(2, HIGH);
          digitalWrite(3, LOW);
          digitalWrite(4, LOW);
          Serial.print("Waa normal guriga xaraaradiisu: ");
      }
      else if(temperature >= baselineTemp + 4 && temperature < baselineTemp + 6)
      {
          digitalWrite(2, HIGH);
          digitalWrite(3, HIGH);
          digitalWrite(4, LOW);
          Serial.print("Gurigayga xaraaradiisu waa soo kululaanaysaa, dariishadda iska fur: ");
      }
      else if(temperature >= baselineTemp + 6)
      {
          digitalWrite(2, HIGH);
          digitalWrite(3, HIGH);
          digitalWrite(4, HIGH);
          Serial.print("Waawareey war waad ku gubanaysaa saan ee dariishadaha iyo albaabkaba iska fur: ");
      }
      
      // float temperature = (voltage - .5) * 100;
      Serial.print("degrees C: ");
      Serial.println(temperature);

      // update tmp value for next loop:
      tmp_update = time + TEMP_DELAY;
    }
    // delay(3000);
}
