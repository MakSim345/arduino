
/*
Arduino SPI DigiPot Control Tutorial:
https://www.youtube.com/watch?v=pp36Q5i08HE&list=PLrLOfmf1dB-gNMG2RBjhx1V51bKhtHI4E&index=45
*/
// include SPI library.
#include "SPI.h"

// When using the SPI library, only worry about select slave, SS.
// By default , 
// 11 - MOSI
// 12 - MISO
// 13 - CLK
const int SS1 = 10; // Slave Select Chip 1
const int SS2 = 9;  // Slave Select Chip 2


const byte REG0 =B00000000;
const byte REG1 =B00010000;


void setup()
{
  // set pin direction for SS:
  pinMode(SS1, OUTPUT);
  pinMode(SS2, OUTPUT);

  // Init SPI:
  SPI.begin();
}

void setLed(int SS, int reg, int level)
{
  digitalWrite(SS, LOW); // set the given SS pin to low
  SPI.transfer(reg); // choose the register to write
  SPI.transfer(level); // set the LED level (0-128)
  digitalWrite(SS, HIGH); // set the given SS pin to low
}

void loop()
{
  for(int i=0; i <= 128; i++)
  {
      setLed(SS1, REG0, i);
      setLed(SS1, REG1, i);
      setLed(SS2, REG0, i);
      setLed(SS2, REG1, i);
      delay(10);;
  }
  delay(300);

  for(int i=128; i >= 0; i--)
  {
      setLed(SS1, REG0, i);
      setLed(SS1, REG1, i);
      setLed(SS2, REG0, i);
      setLed(SS2, REG1, i);
      delay(10);;
  }
  delay(300);
}