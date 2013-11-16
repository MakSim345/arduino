/*
* 
  test of the 74HC164 Serial to parallel converter
  The 8 LEDs light for one second exactly on each pass
  Arduino NANO.
*/

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

int _arr[10] = {_zer, _one, _two, _tre, _for, _fiv, _six, _sev, _eit, _nin};

void setup()                     
{
  pinMode(clockPin, OUTPUT); 
  pinMode(dataPin, OUTPUT); 
  //Serial.begin(9600);           // init serial
  // shiftOut(dataPin, clockPin, LSBFIRST, B10101010);
  shiftOut(dataPin, clockPin, LSBFIRST, B00000000);
}

void loop()
{
    //FastForward();
    //Reverce();
    for(int i=0; i<10; i++)
    {
      shiftOut(dataPin, clockPin, LSBFIRST, _arr[i]);
      delay(_my_delay);
    }   
  shiftOut(dataPin, clockPin, LSBFIRST, B00000000);
  delay(1000);
  
  
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