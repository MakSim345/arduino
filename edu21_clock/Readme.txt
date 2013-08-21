//example use of LCD4Bit_mod library

// #include <LCD4Bit_mod.h> 
//create object to control an LCD.  
//number of lines in display=1
// LCD4Bit_mod lcd = LCD4Bit_mod(2); 
#include "seven_digit_led.h"

seven_digit_led *sdl;

// Defines
long MINS = 60;         // 60 Seconds in a Min.
long HOURS = 60 * MINS; // 60 Mins in an hour.
long DAYS = 24 * HOURS; // 24 Hours in a day. > Note: change the 24 to a 12 for non millitary time.

long runTime = 0;       // Time from when we started.
long clockHourSet = 12;
long clockMinSet  = 59;

int adc_key_val[5] ={ 30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;
int dotPosition;


char dateString[30];

void setup() 
{ 
  //lcd.init();
  
  //optionally, now set up our application-specific display settings, overriding whatever the lcd did in lcd.init()
  //lcd.commandWrite(0x0F);//cursor on, display on, blink on.  (nasty!)
  
  //lcd.clear();
  //lcd.printIn("                          ");
    sdl = new seven_digit_led(4);
    // nextChange = 0;
    Serial.begin(9600);
    Serial.print("time begin: \n");
}

void handleKeyPress()
{
  adc_key_in = analogRead(0);
  key = get_key(adc_key_in);
  if (key >=0){
    if (key == 1){
      clockHourSet++;
    }
    else if (key == 2)
    {
      clockHourSet--;
    }
    else if (key == 0)
    {
      clockMinSet++;
    }
    else if (key == 3)
    {
      clockMinSet--;
    }     
  }
  key = -1;
}

// Convert ADC value to key number
int get_key(unsigned int input)
{
  int k;
  for (k = 0; k < NUM_KEYS; k++)
  {
    if (input < adc_key_val[k])
    {  
      return k;
    }
  }
  if (k >= NUM_KEYS)
    k = -1;     // No valid key pressed   
  return k;
}

void updateTimeString()
{
  runTime = millis();

  long time = (runTime) / 1000;

  long hbump = 60*60*clockHourSet;
  long mbump = 60*clockMinSet;
  time += mbump + hbump;

  long days  = time / DAYS;    
  time -= days  * DAYS; 
  long hours = time / HOURS;   
  time -= hours * HOURS; 
  long minutes  = time / MINS;    
  time -= minutes  * MINS; 
  long seconds  = time; 

  int lowerHours = hours % 10;
  int upperHours = hours - lowerHours;
  int lowerMins = minutes % 10;
  int upperMins = minutes - lowerMins;
  int lowerSeconds = seconds % 10;
  int upperSeconds = seconds - lowerSeconds;
  if( upperSeconds >= 10 )   upperSeconds = upperSeconds / 10;
  if( upperMins >= 10 )      upperMins = upperMins / 10;
  if( upperHours >= 10 )     upperHours = upperHours / 10;
  
  if(lowerSeconds%2)
  {
    sprintf(dateString," Time: %d%d %d%d %d%d",upperHours,lowerHours,upperMins,lowerMins,upperSeconds,lowerSeconds);
  }
  else
  {
    sprintf(dateString," Time: %d%d:%d%d:%d%d",upperHours,lowerHours,upperMins,lowerMins,upperSeconds,lowerSeconds);
  }
  
  //draw dot
  if (lowerMins%2)
  {
    dotPosition++;
    if (dotPosition>30)
    {
      dotPosition=0;
    }  
   
    //lcd.cursorTo(1, dotPosition/2);  //line=2, x=0
    //lcd.printIn(" C‚");
  }
  else
  {
    //lcd.cursorTo(1, dotPosition/2);  //line=2, x=0
    //lcd.printIn("  ");
    dotPosition=0;
  }
}



void loop() 
{
  handleKeyPress();
  updateTimeString();
  //lcd.cursorTo(2, 0);  //line=2, x=0
  //lcd.printIn(dateString);
  Serial.print(dateString);
  Serial.print("\n");
  delay(100);
}





