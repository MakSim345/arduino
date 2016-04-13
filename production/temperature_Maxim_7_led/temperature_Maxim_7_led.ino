//We always have to include the library
#include "LedControl.h"
#include <Time.h> 

// comment it out if LM35 is in use!
#define TM36_IN_USE

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

LedControl lc=LedControl(DATA_IN_PIN, CLK_PIN, LOAD_PIN, 1);

#define DEVICE 0

#define VCCPIN 8     // 

/* Create buffers */
char chr_buf[50];
char chr_day[10];

char dateString[30];
const int DIGIT_DELAY = 10000; // 2 sec
const int NUM_DIGITS = 1000;
long tmp_update = 0; // temperature
long sensorVal=0; //variable to store the value coming from the sensor
int temperature = 0;  //variable which will be calculated in process
long val=0; //variable to store the value coming from the sensor
/* we always wait a bit between updates of the display */
unsigned long delaytime=250;
unsigned long _ctr = 0;

void setup()
{
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  pinMode(VCCPIN, OUTPUT);
  digitalWrite(VCCPIN, HIGH);
  pinMode(POT_PIN, INPUT); 

  // Turn the Serial Protocol ON
  Serial.begin(9600);
#ifdef TM36_IN_USE
    Serial.println("TM36 Thermometer - Start.");
#else
    Serial.println("LM35 Thermometer - Start.");
#endif    
  
  int _device = DEVICE;
  lc.shutdown(_device, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(_device, 10);
  /* and clear the display */
  lc.clearDisplay(_device);
}

void displayNumber(unsigned long value)
{
   byte decimal[8] = {0};
   value = value % 100000000;  //ensure the value is within 8 digits only
   decimal[7] = value / 10000000;  //extract digit 7 from value
   value = value % 10000000;       //extract the rest of 7 digit value
   decimal[6] = value / 1000000;
   value = value % 1000000;
   decimal[5] = value / 100000;
   value = value % 100000;
   decimal[4] = value / 10000;
   value = value % 10000;
   decimal[3] = value / 1000;
   value = value % 1000;
   decimal[2] = value / 100;
   value = value % 100;
   decimal[1] = value / 10;
   decimal[0] = value % 10;
   byte zero = 0;
   int _num = 0;

   for(int i=0; i<8; i++)
   {
     lc.setDigit(0, i, decimal[i], false);
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

void loop()
{
  //displayNumber(_ctr);
  long time = millis();
  if (time >= tmp_update) 
  {
    tmp_update = time + DIGIT_DELAY;    

    sensorVal = analogRead(POT_PIN); //read the value of sensor
#ifdef TM36_IN_USE
    float voltage = (sensorVal/1024.0) * 5.0;
    float temperature = (voltage - .5) * 100;
    Serial.print("Sensor TM36 temperature: ");
#else                
    // Volts at pin in milliVolts = (reading from ADC) * (5000/1024) 
    temperature = (sensorVal)*(500/1024.0); //convert volts to temperature
    Serial.print("Sensor LM35 temperature: ");
#endif        
    Serial.print((float)temperature);
    Serial.print("\n");

    disp_right_lcd(temperature);
    // displayNumber(temperature*100.0);
    // displayNumber(1234);
    // delay(1000);                                     //waiting a second
  } 
}
