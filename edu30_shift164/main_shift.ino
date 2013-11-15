/*
* test of the 74HC164 Serial to parallel converter
The 8 LEDs light for one second exactly on each pass
*/

#define BUTTON 2
#define LED 13
#define TIME_CHATTER 5

const int clockPin = 9; // clock sequence
const int dataPin = 8; // Serial data line 


int val=125;

void setup()                     
{
  pinMode(clockPin, OUTPUT); 
  pinMode(dataPin, OUTPUT); 
  //Serial.begin(9600);           // init serial
  shiftOut(dataPin, clockPin, LSBFIRST, B10101010);

}

void loop()
{
  for(int i=0; i<8; i++)
  {
    shiftOut(dataPin, clockPin, MSBFIRST, B00000001 << i);
    delay(val); // 125 * 8 is one second            
  }

  for(int i=0; i<8; i++)
  {
    shiftOut(dataPin, clockPin, MSBFIRST, B10000000 >> i);
    delay(val); // 125 * 8 is one second            
  } 
}
