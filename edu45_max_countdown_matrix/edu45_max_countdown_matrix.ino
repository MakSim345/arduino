// based on an orginal sketch by Arduino forum member "danigom"
// http://forum.arduino.cc/index.php?action=profile;u=188950

#include <avr/pgmspace.h>
#include <LedControl.h>
#include "char_matrix.h"
#include "Time.h"

// #define NANO_IN_USE 
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


const int numDevices = 4;      // number of MAX7219s used
const long scrollDelay = 75;   // adjust scrolling speed
const int DIGIT_DELAY = 5; // 2ms optimal
const int NUM_DIGITS = 1000;
const int TOMATO = 2;//25;
const int BREAK = 1;//5;
const int MONSTERS = 20;//5;

const int TOMATO_IN_RUN = 1;
const int BREAK_IN_RUN = 0;

long nextChange;
int _sec_to_print = 0;
int _cur_sec = 0;
unsigned long delayTime=200;  // Delay between Frames


unsigned long bufferLong [14] = {0};

int flag = TOMATO_IN_RUN;
int timer_min = TOMATO;
int inv_ctr = MONSTERS;
int timer_sec = 0;
int stop_timer = 0;

void(* resetFunc) (void) = 0; //declare reset function @ address 0

LedControl lc=LedControl(12, 11, 10, numDevices);
byte* nums[] = {number0, number1, number2, number3, number4, number5, number6, number7, number8, number9};

void setup_matrix()
{
    for (int x=0; x<numDevices; x++)
    {
        lc.shutdown(x, false);      //The MAX72XX is in power-saving mode on startup
        lc.setIntensity(x, 4 );      // Set the brightness to default value
        lc.clearDisplay(x);         // and clear the display
    }
}

void setup()
{
    setup_matrix();
    Serial.begin(9600);
    setTime(12, 34, 0, 8, 7, 2015); // HH-MM-SS DD-MM-YYYY
    Serial.write("START - \n"); //
    _sec_to_print = now();
}

void loop()
{
  long time = millis();
  if (time >= nextChange)
  {
    nextChange = time + DIGIT_DELAY;
    //print_time();
    /* Get the current time and date from the chip */
    //Time t = rtc.time();
    //int _hour_to_print = hour();
    //int _min_to_print = minute();

    // _cur_sec = second();
     _cur_sec = now();
    if (_sec_to_print < _cur_sec)
    {
        /*
        Serial.write("decrement_timer\n");
        itoa(_cur_sec, v_str, 6);

        Serial.write("_cur: ");
        Serial.write(v_str);
        Serial.write(" ");

        itoa(_sec_to_print, v_str, 6);
        Serial.write("_prt: ");
        Serial.write(v_str);
        Serial.write("--\n");
        */
        decrement_timer();
        _sec_to_print = now();
    }
    if (0 == stop_timer)
    {
        show_min (timer_min);
        show_sec (timer_sec);
    }
    else
    {
        // Put #1 frame on both Display
        sinvader1a();
        delay(delayTime);
        sinvader2a();
        delay(delayTime);

        // Put #2 frame on both Display
        sinvader1b();
        delay(delayTime);
        sinvader2b();
        delay(delayTime);
        inv_ctr = inv_ctr - 1;
        if (inv_ctr <= 0)
        { 
          inv_ctr = 20;
          stop_timer = 0;
          if (flag == TOMATO_IN_RUN)
          {
            flag = BREAK_IN_RUN;
            timer_min = BREAK;
            Serial.write("BREAK_IN_RUN \n"); //
          }            
          else
          {
            flag = TOMATO_IN_RUN;
            timer_min = TOMATO;
            Serial.write("TOMATO_IN_RUN \n"); //
          }
        }

           // resetFunc();
    }
    // sdl->show_number(_hour_to_print * 100 + _min_to_print);
    // sdl->show_number(_min_to_print * 100 + _sec_to_print);
  }
}

void decrement_timer()
{
    if (0 == stop_timer)
    {
        if (timer_sec == 0)
        {
            timer_sec = 59;
            timer_min = timer_min - 1;
            if (timer_min < 0)
            {
                timer_min = 0;
                stop_timer = 1;
            }
        }
        else
        {
            timer_sec = timer_sec - 1;
        }
    }
    else
    {
       //delay(3000);
       // setup_matrix();
       // reset device:
       //resetFunc();
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
    lc.setRow(2, i, nums[decimal[0]][i]);
  }

  for (int i = 0; i < 8; i++)
  {
    lc.setRow(3, i, nums[decimal[1]][i]);
  }

 //lc.setDigit(0, 3, decimal[0], false);
 //lc.setDigit(0, 4, decimal[1], false);
}

void sinvader1a()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, invader1a[i]);
    lc.setRow(2, i, invader1a[i]);
  }
}

void sinvader1b()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, invader1b[i]);
    lc.setRow(2, i, invader1b[i]);
  }
}

void sinvader2a()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(1, i, invader2a[i]);
    lc.setRow(3, i, invader2a[i]);
  }
}

void sinvader2b()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(1, i, invader2b[i]);
    lc.setRow(3, i, invader2b[i]);
  }
}
/*
void show_hour(int hours)
{
  byte decimal[8] = {0};
  decimal[1] = hours / 10;
  decimal[0] = hours % 10;

  lc.setDigit(0, 6, decimal[0], false);
  lc.setDigit(0, 7, decimal[1], false);
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
