/*
Project: Temperature and Humidity meter with seven segment display
Description: Uses the LedControl library and Adafruit's DHT sensor
library for reading temperature and humidity measurements from DHT11
sensor and displaying the result on MAX7219-driven seven segment LED
displays.

Posted on: www.embedded-lab.com
URL of the project: http://embedded-lab.com/blog/?p=8414
*/
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

/*
In this project, we insert the DHT11 sensor directly into the right female headers of the Arduino Uno board such that the pin 4 (GND) of DHT11 goes in to the GND pin (second pin from top right) of the Arduino header. With this arrangement, the pins 1 (VCC), 2 (Data), and 3 (NC) of DHT11 will be connected to Arduino digital I/O pins 11, 12, and 13, respectively. Since DHT11 requires only 2.5mA current at most during operation, it will be powered directly through Arduino pin 11 by setting it to 1. The Arduino pin 12 is used to read the sensor output.
*/
#define DHTPIN 7     // 12 what pin we're connected to
#define VCCPIN 8     // 11
#define DHTTYPE DHT11   // DHT 11

// Connect pin 1 (on the left) of the sensor to Pin 11 of Arduino
// Connect pin 2 of the sensor to whatever your Pin 12 of Arduino
// Connect pin 4 (on the right) of the sensor to GROUND
int tempC, rh;

DHT dht(DHTPIN, DHTTYPE);
// LedControl lc=LedControl(5,7,6,0); // Pin 5 to Data In, 7 to Clk, 6 to LOAD
LedControl lc=LedControl(DATA_IN_PIN, CLK_PIN, LOAD_PIN, 1);

void setup()
{
  pinMode(VCCPIN, OUTPUT);
  digitalWrite(VCCPIN, HIGH);

  Serial.begin(9600);
  Serial.println("DHTxx test!");
  dht.begin();

  lc.shutdown(0, false);// turn off power saving, enables display
  lc.setIntensity(0, 12);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
}

void loop()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  tempC = (int)t;
  rh   = (int)h;
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h))
  {
    Serial.println("Failed to read from DHT");
  }
  else
  {
    Serial.print("Humidity: ");
    Serial.print(rh);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C");
    // Disp_Data();
    disp_right_lcd(tempC);
    delay(1000);
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
  // Now display RH in row 2
  ones = val_to_show%10;
  tens = (val_to_show/10)%10;
  lc.setDigit(0,7,(byte)tens,false);
  lc.setDigit(0,6,(byte)ones,false);
  lc.setChar(0, 5, ' ', false);
  lc.setChar(0, 4, 'P', false);
}

void Disp_Data()
{
  int ones, tens;
  // First display temperature in row 1
  ones = tempC%10;
  tens = (tempC/10)%10;
  lc.setDigit(0,3,(byte)tens,false);
  lc.setDigit(0,2,(byte)ones,false);
  lc.setChar(0, 1, '/', false);
  lc.setChar(0, 0, 'C', false);

  // Now display RH in row 2
  ones = rh%10;
  tens = (rh/10)%10;
  lc.setDigit(0,7,(byte)tens,false);
  lc.setDigit(0,6,(byte)ones,false);
  lc.setChar(0, 5, ' ', false);
  lc.setChar(0, 4, 'P', false);
}
