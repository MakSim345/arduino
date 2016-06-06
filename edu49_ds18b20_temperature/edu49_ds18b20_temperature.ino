#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#include <LedControl.h>

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

void disp_right_lcd(int val_to_show);
void disp_left_lcd(int val_to_show);

#define CE_PIN   7 //4  // RST?
#define IO_PIN   6 //3  // DAT?
#define SCLK_PIN 5 //2  // CLK

int POT_PIN = A0; //input read pin for LM35 is Analog Pin X (AX on the board, not just X!)

#define DHTPIN 7     //  what pin we're connected to
#define VCCPIN 8     //
#define DHTTYPE DHT11   // DHT 11
#define DALLASPIN 7

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

// Connect pin 1 (on the left) of the sensor to VCCPIN of Arduino
// Connect pin 2 of the sensor to whatever your DHTPIN of Arduino
// Connect pin 4 (on the right) of the sensor to GROUND
int tempC, tempC2;
int rh;
DHT dht(DHTPIN,  DHTTYPE);
// DHT dht2(DHTPIN2, DHTTYPE);
LedControl lc=LedControl(DATA_IN_PIN, CLK_PIN, LOAD_PIN, 1);

/*
Сборка устройства:
первую ногу датчика цепляем на «минус» GND
вторую ногу (DQ) на 7-й пин
третью на плюс
вторую и третью нужно соединить резистором на ~ 4,7 К. Но я заменил резистор на светодиод и получил индикатор обращения к шине датчика (ВНИМАНИЕ! Без резистора или светодиода работать ничего не будет. Не забудьте!)
*/

const int DIGIT_DELAY = 3000; // millisec
long tmp_update = 0; // temperature
long sensorVal=0; //variable to store the value coming from the sensor
int temperature = 0;  //variable which will be calculated in process
long val=0; //variable to store the value coming from the sensor
/* we always wait a bit between updates of the display */
unsigned long delaytime=250;
unsigned long _ctr = 0;

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  // method 1 - slower
  //Serial.print("Temp C: ");
  //Serial.print(sensors.getTempC(deviceAddress));
  //Serial.print(" Temp F: ");
  //Serial.print(sensors.getTempF(deviceAddress)); // Makes a second call to getTempC and then converts to Fahrenheit

  // method 2 - faster
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
}

void setup()
{
  pinMode(VCCPIN, OUTPUT);
  digitalWrite(VCCPIN, HIGH);
  pinMode(POT_PIN, INPUT);

  // Turn the Serial Protocol ON
  Serial.begin(9600);

  lc.shutdown(0, false);// turn off power saving, enables display
  lc.setIntensity(0, 12);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen

  Serial.println("DHT-1 test!");
  dht.begin();
  
  // locate devices on the bus:
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  
  if (!sensors.getAddress(insideThermometer, 0)) 
  {
    Serial.println("Unable to find address for Device 0");   
  }
 
  // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 9);
 
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();
} // setup


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

    // float t2 = dht2.readTemperature();

    rh   = (int)h;
    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t))
    {
        Serial.println("Failed to read from DHT-1 sensor!");
        t = -99;
    }
    
    tempC = (int)t;
    
    sensors.requestTemperatures(); // Send the command to get temperatures
    // printTemperature(insideThermometer); // Use a simple function to print out the data
    
    // float f=-20.7;
    float f = sensors.getTempC(insideThermometer);
    int tempC2 = int (f); // (f - (int)f) * 100; // выделяем дробную часть
    {
        //Serial.print("Humidity: ");
        //Serial.print(rh);
        //Serial.print(" %\t");
        Serial.print("Sensor DHT11(1) temperature: ");
        Serial.print(t);
        Serial.println(" *C");
        disp_right_lcd(tempC);

        Serial.print("Sensor -DALLAS- temperature: ");
        Serial.print(tempC2);
        Serial.println(" *C");        
        disp_left_lcd(tempC2);
    }
  } // if (time >= tmp_update)
} // loop

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


