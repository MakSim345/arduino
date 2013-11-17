/*
* 
  test of the 74HC164 Serial to parallel converter
  The 8 LEDs light for one second exactly on each pass
  Arduino NANO.
*/
#define DIGIT_ON  HIGH//LOW
#define DIGIT_OFF LOW//HIGH
#define DISPLAY_BRIGHTNESS 3500

#define BUTTON 2
#define LED 13
#define TIME_CHATTER 5

const int clockPin = 9; // clock sequence
const int dataPin = 8; // Serial data line 

void FastForward();
void Reverce();


const byte DIGITS[10][8] = {
    //A  B  C  D  E  F  G  H
    { 1, 1, 1, 1, 1, 1, 0, 0 }, // 0
    { 0, 1, 1, 0, 0, 0, 0, 0 }, // 1
    { 1, 1, 0, 1, 1, 0, 1, 0 }, // 2
    { 1, 1, 1, 1, 0, 0, 1, 0 }, // 3 
    { 0, 1, 1, 0, 0, 1, 1, 0 }, // 4
    { 1, 0, 1, 1, 0, 1, 1, 0 }, // 5
    { 1, 0, 1, 1, 1, 1, 1, 0 }, // 6
    { 1, 1, 1, 0, 0, 0, 0, 0 }, // 7
    { 1, 1, 1, 1, 1, 1, 1, 0 }, // 8
    { 1, 1, 1, 1, 0, 1, 1, 0 }  // 9
    };


int val=125;
int _my_delay = 500;
/*
int _zer = B11111100;
int _one = B01100000;
int _two = B11011010;
int _tre = B11110010;
int _for = B01100110;
int _fiv = B10110110;
int _six = B10111110;
int _sev = B11100000;
int _eit = B11111110;
int _nin = B11110110;
*/
int _zer = B00000011;
int _one = B10011111;// B01100000;
int _two = B00100101;//B11011010;
int _tre = B00001101;//B11110010;
int _for = B10011001;//B01100110;
int _fiv = B01001001;//B10110110;
int _six = B01000001;//B10111110;
int _sev = B00011111;//B11100000;
int _eit = B00000001;//B11111110;
int _nin = B00001001;//B11110110;

int _arr[10] = {_zer, _one, _two, _tre, _for, _fiv, _six, _sev, _eit, _nin};

long nextChange;
const int NUM_DIGITS = 1000;
const int DIGIT_DELAY = 5; // 2ms optimal
int currentDigit = 0;

unsigned short hours, minutes, seconds;
boolean pm;
unsigned long lastTime; // keeps track of when the previous second happened

void setup_time()
{
  // set the initial time
  hours = 3;
  minutes = 23;
  seconds = 0;
  pm = true;

  lastTime = millis();
}

void setup()                     
{
  setup_time();
  pinMode(clockPin, OUTPUT); 
  pinMode(dataPin, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  nextChange = 0;
  //Serial.begin(9600);           // init serial
  // shiftOut(dataPin, clockPin, LSBFIRST, B10101010);
  //shiftOut(dataPin, clockPin, LSBFIRST, B00000000);
  //digitalWrite(2, HIGH);
}

void loop()
{
    long time = millis();
    // Keep showing the display while waiting for timer to expire  
    while (millis() - lastTime < 1000) 
    {
      show_number(hours*100 + minutes);
    }

    lastTime += 1000;  
    incrementTime();
/*
    if (time >= nextChange) 
    {
        currentDigit = (millis()/1000) % NUM_DIGITS;
        show_number(currentDigit);
        nextChange = time + DIGIT_DELAY;
    }
*/
    //FastForward();
    //Reverce();
    // show_number(2);
    //delay(1000);
    //show_number(1256);
    //delay(1000);
    /*
    for(int i=0; i<10; i++)
    {
      shiftOut(dataPin, clockPin, LSBFIRST, _arr[i]);
      delay(_my_delay);
    }   
    shiftOut(dataPin, clockPin, LSBFIRST, B1111111);
    delay(1000);
  */
  
  /*
  for(int i=0; i<8; i++)
  {  
    digitalWrite(clockPin, LOW);  
    digitalWrite(dataPin, DIGITS[1][i]);
    digitalWrite(clockPin, HIGH);
    delay(500);
  }
 */

  /*
  for(int i=0; i<9; i++)
  {
    // shiftOut(dataPin, clockPin, MSBFIRST, B00000001 << i);
    digitalWrite(clockPin, HIGH);  
    digitalWrite(dataPin, 1);
    digitalWrite(clockPin, LOW);  
    delay(1000); // 125 * 8 is one second            
  }
  */
}

void writeNumber(int in_value)
{
  shiftOut(dataPin, clockPin, LSBFIRST, _arr[in_value]);
  //delay(3);
}

void show_number(int num)
{ 
    int current_pin;
    int digitONE;
    int digitTWO;
    int digitTHREE;
    int digitFOUR;

    digitONE  = num % 10;
    digitTWO = num/10 - 10*(num/100);
    digitTHREE = num/100 - 10*(num/1000);
    digitFOUR = num/1000;
    //   = (currentDigit - currentDigit % 10)/10;
    
    for(int digit = 4; digit > 0 ; digit--) 
    {
    //Turn on a digit for a short amount of time
    //REM: digits are ordered backwise!
    switch(digit) 
        {
        case 1:
            current_pin = 2;
            digitalWrite(current_pin, DIGIT_ON);
            writeNumber(digitFOUR);
            break;
        case 2:
            current_pin = 3;
            digitalWrite(current_pin, DIGIT_ON);
            writeNumber(digitTHREE);
            break;
        case 3:
            current_pin = 4;
            digitalWrite(current_pin, DIGIT_ON);
            writeNumber(digitTWO);
            break;
        case 4:
            current_pin = 5;
            digitalWrite(current_pin, DIGIT_ON);
            writeNumber(digitONE);
            break;
        }

        // wait for fix an image:
        delayMicroseconds(DISPLAY_BRIGHTNESS);
        // reset all digits for refresh:
        // writeNumber(ERASE_SEGMENTS);
        digitalWrite(current_pin, DIGIT_OFF);
    }
}


//
// a call to incrementTime increases time by one second.
//
void incrementTime() 
{
  if (seconds == 59) 
  {
    seconds = 0;
    
    if (minutes == 59) 
    {
      minutes = 0;
      
      if (hours == 12) 
      {          
        hours = 1;
      }
      else 
      {
        hours++;
        
        if (hours == 12) 
        {
          pm = !pm;
        }
      }
    }
    else 
    {
      minutes++;
    }
  }
  else 
  {
    seconds++;  
  }
}

void FastForward()
{
  for(int i=0; i<8; i++)
  {
    shiftOut(dataPin, clockPin, MSBFIRST, B00000001 << i);
    delay(val); // 125 * 8 is one second            
  }
}

void Reverce()
{
  for(int i=0; i<8; i++)
  {
    shiftOut(dataPin, clockPin, MSBFIRST, B10000000 >> i);
    delay(val); // 125 * 8 is one second            
  }
}