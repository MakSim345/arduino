// based on an orginal sketch by Arduino forum member "danigom"
// http://forum.arduino.cc/index.php?action=profile;u=188950
 
// #include <avr/pgmspace.h>
#include <Wire.h>
 #include <LedControl.h>
#include "char_matrix.h"
#include "RTClib.h"
//#include "DS1302.h"
// #include "Time.h" 


#define NANO_IN_USE 
// #define ARDUINO_IN_USE 
/*
For RTC DS1307 in NANO:
SCL is pin A5 and SDA is pin A4 on the Nano
SCL = A5
SDL = A4

Now we need a LedControl to work with.
***** These pin numbers will probably not work with your hardware *****
pin 12 is connected to the DataIn, DIN
pin 11 is connected to the CLK
pin 10 is connected to LOAD
We have only a single MAX72XX.
*/
#ifdef ARDUINO_IN_USE
    // it is LED control!
    #define DATA_IN_PIN 12
    #define CLK_PIN 11
    #define LOAD_PIN 10
#endif
/*
 Arduino NANO, pin #XX 
 */
#ifdef NANO_IN_USE 
    // it is LED control!
    #define DATA_IN_PIN 5  // (D5)
    #define CLK_PIN     12 // (D9)
    #define LOAD_PIN    13 // (D13)
#endif

/* Set the appropriate digital I/O pin connections
uint8_t CE_PIN   = 13; //5; // RST?
uint8_t IO_PIN   = A1; //6; // DAT?
uint8_t SCLK_PIN = A0; //7; // CLK
*/

const int MATRIX_NUMBER = 4;   // define number of MAX7219s used
const long scrollDelay = 75;   // adjust scrolling speed
const int DIGIT_DELAY = 500; 
const int MATRIX_BRIGHTNESS = 1;  // 4 - is the best
const int NUM_DIGITS = 1000;
long nextChange;
 
unsigned long bufferLong [14] = {0};

const char *months[] = {"Jan", "Feb", "Mar", 
                        "Apr", "May", "Jun", 
                        "Jul", "Aug", "Sep", 
                        "Oct", "Nov", "Dec" };


LedControl lc=LedControl(DATA_IN_PIN, CLK_PIN, LOAD_PIN, MATRIX_NUMBER);

byte* nums[] = {number0, number1, number2, number3, number4, number5, number6, number7, number8, number9};

RTC_DS1307 RTC;

void setup()
{    
  // Turn the Serial Protocol ON  
  // Uncomment this function if new time has to be set to RTC:
  // setTimeInRTC();  

  for (int curMax=0; curMax < MATRIX_NUMBER; curMax++)
  {
    lc.shutdown(curMax, false);      //The MAX72XX is in power-saving mode on startup
    lc.setIntensity(curMax, MATRIX_BRIGHTNESS);      // Set the brightness to default value
    lc.clearDisplay(curMax);         // and clear the display
  }
  // setTime(10, 50, 0, 12, 10, 2015); // HH-MM-SS DD-MM-YYYY

  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  
  if (! RTC.isrunning()) 
  {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}
 
void loop()
{
  long time = millis();
  if (time >= nextChange) 
  {
    nextChange = time + DIGIT_DELAY;
    //print_time();
    /* Get the current time and date from the chip */
    // Time t = rtc.time();

    DateTime now = RTC.now();

    int _hour_to_print = now.hour(); //hour();
    int _min_to_print = now.minute(); //min(); 
    int _sec_to_print = now.second(); //sec();
    
    Serial.print(now.day());
    Serial.print('-');
    // Serial.print(now.month());
    int mon_num = now.month();
    Serial.print (months[mon_num-1]);

    Serial.print('-');
    Serial.println(now.year());

    Serial.print(now.hour());
    Serial.print(':');
    Serial.print(now.minute());
    Serial.print(':');
    Serial.println(now.second());

    delay (1000);
    // _sec_to_print++;
    //int _hour_to_print = hour();
    //int _min_to_print = minute(); 
    //int _sec_to_print = second(); 
              
    show_hour(_hour_to_print);
    show_min (_min_to_print);
    //show_sec (_sec_to_print);    
  }
}


void getdate(int& yearP, int& monthP, int& dayP)
{
    char temp [] = __DATE__; // "Jan  8 2021"
    
    const int MILLENIUM = 2000;
    unsigned char i;

    yearP = atoi(temp + 9) + MILLENIUM;
    *(temp + 6) = 0;

    dayP = atoi(temp + 4);
    *(temp + 3) = 0;

    for (i = 0; i < 12; i++)
    {
        if (!strcmp(temp, months[i]))
        {
            monthP = i + 1;
            return;
        }
    }
}
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Take values in Arrays and Display them
void show_sec(int seconds)
{
  byte decimal[8] = {0};
  decimal[1] = seconds / 10;
  decimal[0] = seconds % 10;
  
  for (int i = 0; i < 8; i++)  
  {
    lc.setRow(0, i, nums[decimal[0]][i]);
  }
  
  for (int i = 0; i < 8; i++)  
  {
    lc.setRow(1, i, nums[decimal[1]][i]);
  }
  
  //lc.setDigit(0, 0, decimal[0], false);
  //lc.setDigit(0, 1, decimal[1], false);
}

void show_min(int minutes)
{
  byte decimal[8] = {0};
  decimal[1] = minutes / 10;
  decimal[0] = minutes % 10;
  
  for (int i = 0; i < 8; i++)  
  {
    lc.setRow(0, i, nums[decimal[0]][i]);
  }
  
  for (int i = 0; i < 8; i++)  
  {
    lc.setRow(1, i, nums[decimal[1]][i]);
  }

 //lc.setDigit(0, 3, decimal[0], false);
 //lc.setDigit(0, 4, decimal[1], false);
}

void show_hour(int hours)
{
  byte decimal[8] = {0};
  decimal[1] = hours / 10;
  decimal[0] = hours % 10;
  
  for (int i = 0; i < 8; i++)  
  {
    lc.setRow(2, i, nums[decimal[0]][i]);
  }
  
  for (int i = 0; i < 8; i++)  
  {
    lc.setRow(3, i, nums[decimal[1]][i]);
  }
}

/*
void sinvader1a()
{
  for (int i = 0; i < 8; i++)  
  {
    lc.setRow(0, i, invader1a[i]);
  }
}
 
void sinvader1b()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, invader1b[i]);
  }
}
 
void sinvader2a()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(1, i, invader2a[i]);
  }
}
 
void sinvader2b()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(1, i, invader2b[i]);
  }
}
*/


void scrollFont() 
{
    for (int counter=0x20;counter<0x80;counter++)
    {
        loadBufferLong(counter);
        delay(500);
    }
}
 
// Scroll Message
void scrollMessage(const prog_uchar * messageString) 
{
    int counter = 0;
    int myChar=0;
    do {
        // read back a char
        myChar =  pgm_read_byte_near(messageString + counter);
        if (myChar != 0)
        {
            loadBufferLong(myChar);
        }
        counter++;
    }
    while (myChar != 0);
}
// Load character into scroll buffer
void loadBufferLong(int ascii)
{
    if (ascii >= 0x20 && ascii <=0x7f)
    {
        for (int a=0;a<7;a++)
        {                      
           // Loop 7 times for a 5x7 font
            unsigned long c = pgm_read_byte_near(font5x7 + ((ascii - 0x20) * 8) + a);     // Index into character table to get row data
            unsigned long x = bufferLong [a*2];     // Load current scroll buffer
            x = x | c;                              // OR the new character onto end of current
            bufferLong [a*2] = x;                   // Store in buffer
        }
        
        byte count = pgm_read_byte_near(font5x7 +((ascii - 0x20) * 8) + 7);     // Index into character table for kerning data
        
        for (byte x=0; x<count; x++)
        {
            rotateBufferLong();
            printBufferLong();
            delay(scrollDelay);
        }
    }
}
// Rotate the buffer
void rotateBufferLong()
{
    for (int a=0;a<7;a++)
    {                                           // Loop 7 times for a 5x7 font
        unsigned long x = bufferLong [a*2];     // Get low buffer entry
        byte b = bitRead(x,31);                 // Copy high order bit that gets lost in rotation
        x = x<<1;                               // Rotate left one bit
        bufferLong [a*2] = x;                   // Store new low buffer
        x = bufferLong [a*2+1];                 // Get high buffer entry
        x = x<<1;                               // Rotate left one bit
        bitWrite(x,0,b);                        // Store saved bit
        bufferLong [a*2+1] = x;                 // Store new high buffer
    }
}  
// Display Buffer on LED matrix
void printBufferLong()
{
  for (int a=0;a<7;a++)
  {                                         // Loop 7 times for a 5x7 font
    unsigned long x = bufferLong [a*2+1];   // Get high buffer entry
    byte y = x;                             // Mask off first character
    lc.setRow(3,a,y);                       // Send row to relevent MAX7219 chip
    x = bufferLong [a*2];                   // Get low buffer entry
    y = (x>>24);                            // Mask off second character
    lc.setRow(2,a,y);                       // Send row to relevent MAX7219 chip
    y = (x>>16);                            // Mask off third character
    lc.setRow(1,a,y);                       // Send row to relevent MAX7219 chip
    y = (x>>8);                             // Mask off forth character
    lc.setRow(0,a,y);                       // Send row to relevent MAX7219 chip
  }
}
