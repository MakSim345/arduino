//We always have to include the library
#include "LedControl.h"
#include "DS1302.h"
#include <Time.h> 


//#define NANO_IN_USE 
#define ARDUINO_IN_USE 
// #define TIME_FROM_RTC 

/*
Now we need a LedControl to work with.
pin 12 is connected to the DataIn, DIN
pin 11 is connected to the CLK
pin 10 is connected to LOAD / CS
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

#define VIBRO_PIN 13 

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

int _hour_to_print;
int _min_to_print; 
int _sec_to_print; 
int tmp_sec = 0;
Time t;

// Create a DS1302 object
// DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN);


void setTimeInRTC();
void print_time();

void setup()
{
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  // Turn the Serial Protocol ON
  Serial.begin(9600);
  pinMode(VIBRO_PIN, OUTPUT); 
  int _device = DEVICE;
  lc.shutdown(_device, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(_device, 10);
  /* and clear the display */
  lc.clearDisplay(_device);
  
  // Uncomment this function if new time has to be set to RTC:
  //setTimeInRTC();

  // Get time from the RTC module:  
  // t = rtc.time();
  // rtc.write_protect(true);

  // DateTime now = DateTime(__DATE__, __TIME__);

#ifdef TIME_FROM_RTC
  int _hr = t.hr;
  int _min = t.min;
  int _sec = t.sec;
  
  int _mon = t.mon;
  int _day = t.day;
  int _year = t.yr;
#endif

  sscanf(__TIME__, "%2hhd %*c %2hhd %*c %2hhd", &t.hr, &t.min, &t.sec);
  int _hr = t.hr;
  int _min = t.min;
  int _sec = t.sec;
  // init internal timer:  
  setTime(_hr, _min, _sec, 1, 1, 2016); // HH-MM-SS DD-MM-YYYY
  // setTime(_hr, _min, _sec, _day, _mon, _year); // HH-MM-SS DD-MM-YYYY
  // setTime(10, 50, 0, 12, 10, 2015); // HH-MM-SS DD-MM-YYYY
}

void loop()
{
  /*
  displayNumber(_ctr);
  delay(10);
  _ctr = _ctr + 1;
  */
  //writeArduinoOn7Segment();
  //scrollDigits();
   
  long time = millis();  
  if (time >= nextChange) 
  {
    nextChange = time + DIGIT_DELAY;
    print_time();

#ifdef TIME_FROM_RTC
    // Get the current time and date from the chip 
    //Time t = rtc.time();
    t = rtc.time();
    _hour_to_print = t.hr; //hour();
    _min_to_print = t.min; //min(); 
    _sec_to_print = t.sec; //sec(); 
#else    
    _hour_to_print = hour();
    _min_to_print  = minute(); 
    _sec_to_print  = second();    
#endif              
    
    if (tmp_sec != _hour_to_print)
    {
      
      tmp_sec = _hour_to_print;
      digitalWrite(VIBRO_PIN, HIGH);
    }
    else
    {
      digitalWrite(VIBRO_PIN, LOW);
    }

    show_hour(_hour_to_print);
    show_min (_min_to_print);
    show_sec (_sec_to_print);

    Serial.print(__TIME__);
    Serial.println(':');
    //Serial.print(__DATE__);
    //Serial.print(':');
    //Serial.println(now.second());

    // sdl->show_number(_hour_to_print * 100 + _min_to_print);
    // sdl->show_number(_min_to_print * 100 + _sec_to_print);
  }   
}

time_t cvt_date(char const *date, char const *time)
{
    char s_month[5];
    int year;
    tmElements_t t;
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

    sscanf(date, "%s %hhd %d", s_month, &t.Day, &year);
    sscanf(time, "%2hhd %*c %2hhd %*c %2hhd", &t.Hour, &t.Minute, &t.Second);

    // Find where is s_month in month_names. Deduce month value.
    t.Month = (strstr(month_names, s_month) - month_names) / 3 + 1;

    // year can be given as '2010' or '10'. It is converted to years since 1970
    if (year > 99) t.Year = year - 1970;
    else t.Year = year + 30;

    return makeTime(t);
}



/*
This method will display the characters for the
word "Arduino" one after the other on digit 0.
*/
void writeArduinoOn7Segment()
{
  int _device = DEVICE;
  lc.setChar(_device, 1, 'a', false);
  delay(delaytime);
  lc.setRow(_device, 0, 0x05);
  delay(delaytime);
  lc.setChar(_device, 0, 'd',false);
  delay(delaytime);
  lc.setRow(_device, 0, 0x1c);
  delay(delaytime);
  lc.setRow(_device, 0, B00010000);
  delay(delaytime);
  lc.setRow(_device, 0, 0x15);
  delay(delaytime);
  lc.setRow(_device, 0, 0x1D);
  delay(delaytime);
  lc.clearDisplay(0);
  delay(delaytime);
}

/*
  This method will scroll all the hexa-decimal
numbers and letters on the display. You will need at least
four 7-Segment digits. otherwise it won't really look that good.
*/
void scrollDigits()
{
  int _num = DEVICE;
  for(int i=0; i<13; i++)
  {
    lc.setDigit(_num, 8, i+1,   false);
    lc.setDigit(_num, 7, i+2,   false);
    lc.setDigit(_num, 6, i+3,   false);
    lc.setDigit(_num, 5, i+4,   false);
    lc.setDigit(_num, 4, i+5,   false);
    lc.setDigit(_num, 3, i+6,   false);
    lc.setDigit(_num, 2, i+7, false);
    lc.setDigit(_num, 1, i+8, false);
    lc.setDigit(_num, 0, i+9, false);
    delay(delaytime);
  }

  lc.clearDisplay(0);
  delay(delaytime);
}

void show_sec(int seconds)
{
  byte decimal[8] = {0};
  decimal[1] = seconds / 10;
  decimal[0] = seconds % 10;
 
  lc.setDigit(0, 0, decimal[0], false);
  lc.setDigit(0, 1, decimal[1], false);
}

void show_min(int minutes)
{
  byte decimal[8] = {0};
  decimal[1] = minutes / 10;
  decimal[0] = minutes % 10;
 
  lc.setDigit(0, 3, decimal[0], false);
  lc.setDigit(0, 4, decimal[1], false);
}

void show_hour(int hours)
{
  byte decimal[8] = {0};
  decimal[1] = hours / 10;
  decimal[0] = hours % 10;
 
  lc.setDigit(0, 6, decimal[0], false);
  lc.setDigit(0, 7, decimal[1], false);
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

void setTimeInRTC()
{
#ifdef TIME_FROM_RTC    
  /* Initialize a new chip by turning off write protection and clearing the
     clock halt flag. These methods needn't always be called. See the DS1302
     datasheet for details. */
    rtc.write_protect(false);
    rtc.halt(false);

  /* Make a new time object to set the date and time 
         YYYY  M  DD  HH  M  S  ?*/
    Time t(2016, 2, 12, 8, 47, 0, 3);

  /* Set the time and date on the chip */
    rtc.time(t);
    rtc.write_protect(true);
#endif    
}

void print_time()
{
  // Get the current time and date from the chip
  /* 
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
  */
}
