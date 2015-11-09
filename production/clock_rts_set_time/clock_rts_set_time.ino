//We always have to include the library
#include "LedControl.h"
#include "DS1302.h"
#include <Time.h> 


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

LedControl lc=LedControl(DATA_IN_PIN, CLK_PIN, LOAD_PIN, 1);

#define DEVICE 0

/* Create buffers */
char chr_buf[50];
char chr_day[10];

char dateString[30];
const int DIGIT_DELAY = 500; // 5 is optimal
const int NUM_DIGITS = 1000;
long nextChange;
long oneSecond;
/* we always wait a bit between updates of the display */
unsigned long delaytime=250;
unsigned long _ctr = 0;

// Create a DS1302 object
DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN);

void setTimeInRTC();
void print_time();

void setup()
{
  // Turn the Serial Protocol ON
  // Serial.begin(9600);
  int _device = DEVICE;
  
  // Uncomment this function if new time has to be set to RTC:
  //setTimeInRTC();

  // Get time from the RTC module:
  Time t = rtc.time();
  int _hr = t.hr;
  int _min = t.min;
  int _sec = t.sec;
  
  int _mon = t.mon;
  int _day = t.day;
  int _year = t.yr;
  
  // init internal timer:  
  setTime(_hr, _min, _sec, _day, _mon, _year); // HH-MM-SS DD-MM-YYYY
}

void loop()
{
  long time = millis();
  if (time >= nextChange) 
  {
    nextChange = time + DIGIT_DELAY;
    print_time();
    
    /* Get the current time and date from the chip */
    //Time t = rtc.time();
    
    int _hour_to_print = t.hr; //hour();
    int _min_to_print = t.min; //min(); 
    int _sec_to_print = t.sec; //sec();

    int _hour_to_print = hour();
    int _min_to_print  = minute(); 
    int _sec_to_print  = second(); 
              
    show_hour(_hour_to_print);
    show_min (_min_to_print);
    show_sec (_sec_to_print);

    // sdl->show_number(_hour_to_print * 100 + _min_to_print);
    // sdl->show_number(_min_to_print * 100 + _sec_to_print);
  } 
}


void setTimeInRTC()
{
    
  /* Make a new time object to set the date and time 
         YYYY  M  DD  HH  M  S  ?*/
  Time t(2015, 10, 20, 10, 15, 0, 3);

  /* Set the time and date on the chip */
  rtc.time(t);
}

void print_time()
{
  // Get the current time and date from the chip
  Time t = rtc.time();

  // Name the day of the week
  memset(chr_day, 0, sizeof(chr_day));  // clear day buffer
  switch (t.day) 
  {
    case 1:
      strcpy(chr_day, "Sunday");
      break;
    case 2:
      strcpy(chr_day, "Monday");
      break;
    case 3:
      strcpy(chr_day, "Tuesday");
      break;
    case 4:
      strcpy(chr_day, "Wednesday");
      break;
    case 5:
      strcpy(chr_day, "Thursday");
      break;
    case 6:
      strcpy(chr_day, "Friday");
      break;
    case 7:
      strcpy(chr_day, "Saturday");
      break;
  }

  // Format the time and date and insert into the temporary buffer
  snprintf(chr_buf, sizeof(chr_buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
           chr_day,
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);

  // Print the formatted string to serial so we can see the time
  Serial.println(chr_buf);
}
