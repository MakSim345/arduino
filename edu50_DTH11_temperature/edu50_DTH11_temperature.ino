#include "DHT.h"
#include "LedControl.h"

#define DECIMAL_NO false
#define DECIMAL_OK true

//#define NANO_IN_USE
#define ARDUINO_IN_USE
/*
Now we need a LedControl to work with.
***** These pin numbers will probably not work with your hardware *****
pin 12 is connected to the DataIn, DIN
pin 11 is connected to the CLK
pin 10 is connected to LOAD
We have only a single MAX72XX.
*/
#ifdef ARDUINO_IN_USE
    #define DATA_IN_PIN 12
    #define CLK_PIN 11
    #define LOAD_PIN 10
#endif
/*
 Arduino NANO, pin #XX
 */
#ifdef NANO_IN_USE
    #define DATA_IN_PIN 5  // (D5)
    #define CLK_PIN     12 // (D9)
    #define LOAD_PIN    13 // (D13)
#endif


#define CE_PIN   7 //4  // RST?
#define IO_PIN   6 //3  // DAT?
#define SCLK_PIN 5 //2  // CLK

int POT_PIN = A0; //input read pin for LM35 is Analog Pin X (AX on the board, not just X!)

#define DHTPIN 7     //  what pin we're connected to
#define VCCPIN 8     //
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN2 2

// Connect pin 1 (on the left) of the sensor to VCCPIN of Arduino
// Connect pin 2 of the sensor to whatever your DHTPIN of Arduino
// Connect pin 4 (on the right) of the sensor to GROUND
int tempC, tempC2;
int rh;
DHT dht(DHTPIN,  DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
LedControl lc=LedControl(DATA_IN_PIN, CLK_PIN, LOAD_PIN, 1);

const int DIGIT_DELAY = 3000; // millisec
long tmp_update = 0; // temperature
long sensorVal=0; //variable to store the value coming from the sensor
int temperature = 0;  //variable which will be calculated in process
long val=0; //variable to store the value coming from the sensor
/* we always wait a bit between updates of the display */
unsigned long delaytime=250;
unsigned long _ctr = 0;

void setup()
{
  pinMode(VCCPIN, OUTPUT);
  digitalWrite(VCCPIN, HIGH);
  pinMode(POT_PIN, INPUT);

  // Turn the Serial Protocol ON
  Serial.begin(9600);

  Serial.println("DHT-1 test!");
  dht.begin();

  Serial.println("DHT-2 test!");
  dht2.begin();

  lc.shutdown(0, false);// turn off power saving, enables display
  lc.setIntensity(0, 12);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
}

void loop()
{
  //displayNumber(_ctr);
  long time = millis();
  if (time >= tmp_update)
  {
    tmp_update = time + DIGIT_DELAY;


    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    float t2 = dht2.readTemperature();

    rh   = (int)h;
    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t))
    {
        Serial.println("Failed to read from DHT-1 sensor!");
        t = -99;
    }
    else if (isnan(t2))
    {
        Serial.println("Failed to read from DHT-2 sensor!");
        t2 = -99;
    }

    tempC = (int)t;
    tempC2= (int)t2;
    
    {
        //Serial.print("Humidity: ");
        //Serial.print(rh);
        //Serial.print(" %\t");
        Serial.print("Sensor DHT11(1) temperature: ");
        Serial.print(t);
        Serial.println(" *C");
        disp_right_lcd(tempC);

        Serial.print("Sensor DHT11(2) temperature: ");
        Serial.print(t2);
        Serial.println(" *C");
        disp_left_lcd(tempC2);
    }
 }
}


void disp_right_lcd(int val_to_show)
{
  int ones, tens;
  int tmp_val;
  int minus_pos = 0;

  if (val_to_show < 0)
    tmp_val = val_to_show*(-1); // make positive to show
  else
    tmp_val = val_to_show;

  ones = tmp_val%10;
  tens = (tmp_val/10)%10;

  if (0 == tens)
  {
    lc.setDigit(0, 2,  ' ', DECIMAL_NO);
    minus_pos = 2;
  }
  else
  {
    lc.setDigit(0, 2, (byte)tens, DECIMAL_NO);
    minus_pos = 3;
  }

  if (val_to_show < 0)
  {
    lc.setChar(0,  minus_pos, '-', DECIMAL_NO);
  }
  else
  {
    lc.setChar(0,  minus_pos, ' ', DECIMAL_NO);
  }

  lc.setDigit(0, 1, (byte)ones, DECIMAL_NO);

  // show last char as degree
  lc.setChar(0,  0, 'C', DECIMAL_NO);
}

void disp_left_lcd(int val_to_show)
{
  int ones, tens;
  int tmp_val;
  int minus_pos = 0;

  if (val_to_show < 0)
    tmp_val = val_to_show*(-1); // make positive to show
  else
    tmp_val = val_to_show;

  ones = tmp_val%10;
  tens = (tmp_val/10)%10;

  if (0 == tens)
  {
    lc.setDigit(0, 2+4,  ' ', DECIMAL_NO);
    minus_pos = 2+4;
  }
  else
  {
    lc.setDigit(0, 2+4, (byte)tens, DECIMAL_NO);
    minus_pos = 3+4;
  }

  if (val_to_show < 0)
  {
    lc.setChar(0,  minus_pos, '-', DECIMAL_NO);
  }
  else
  {
    lc.setChar(0,  minus_pos, ' ', DECIMAL_NO);
  }

  lc.setDigit(0, 1+4, (byte)ones, DECIMAL_NO);

  // show last char as degree
  lc.setChar(0,  0+4, 'C', DECIMAL_NO);
}


/*
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
}
*/
