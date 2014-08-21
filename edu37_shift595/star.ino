#include <SPI.h>
//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;
int nFlag = 1;

void setup() 
{
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

/* Эта функция сдвигает биты влево на одну позицию, перемещая старший бит
 * на место младшего. Другими словами, она "вращает" биты по кругу.
 * Например, 11110000 превращается в 11100001.
 */
void rotateLeft(uint8_t &bits)
{
  uint8_t high_bit = bits & (1 << 7) ? 1 : 0;
  bits = (bits << 1) | high_bit;
}

void star_two()
{
  static uint8_t nomad = 1; // это наш бегающий бит
  // take the latchPin low so 
  // the LEDs don't change while you're sending in bits:
  digitalWrite(latchPin, LOW);
  // shift out the bits:
  shiftOut(dataPin, clockPin, MSBFIRST, nomad);  

  //take the latch pin high so the LEDs will light up:
  digitalWrite(latchPin, HIGH);
  rotateLeft(nomad);
}

void star_one()
{
  // count from 0 to 255 and display the number 
  // on the LEDs
  for (int numberToDisplay = 0; numberToDisplay < 256; numberToDisplay++) 
    {
      // take the latchPin low so 
      // the LEDs don't change while you're sending in bits:
      digitalWrite(latchPin, LOW);
      // shift out the bits:
      shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);  

      //take the latch pin high so the LEDs will light up:
      digitalWrite(latchPin, HIGH);
      // pause before next value:
      delay(500);
    }
}    

void updateShiftRegister(int leds)
{
  digitalWrite(latchPin, LOW);
  // shiftOut(dataPin, clockPin, LSBFIRST, leds);
  shiftOut(dataPin, clockPin, MSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
}

void loop() 
{  
  byte leds = 0;
  
  if (nFlag == 1)
  {
    nFlag = 0;
    updateShiftRegister(128);
    /*for (leds = 0; leds < 8; leds++)
    {
      updateShiftRegister(leds);
      delay(1000);
      // leds++;
    } 
    */ 
  }
  

  /*
  for (int i = 0; i < 8; i++)
  {
    bitSet(leds, i);
    updateShiftRegister(leds);
    delay(1000);
  }
  */
  // pause before next value:  
  // delay(500);  

}