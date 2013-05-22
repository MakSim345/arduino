/*
  Digital Pot Control
  
  This example controls an Analog Devices AD5206 digital potentiometer.
  The AD5206 has 6 potentiometer channels. Each channel's pins are labeled
  A - connect this to voltage
  W - this is the pot's wiper, which changes when you set it
  B - connect this to ground.
 
 The AD5206 is SPI-compatible,and to command it, you send two bytes, 
 one with the channel number (0 - 5) and one with the resistance value for the
 channel (0 - 255).  
 
 The circuit:
  * All A pins  of AD5206 connected to +5V
  * All B pins of AD5206 connected to ground
  * An LED and a 220-ohm resisor in series connected from each W pin to ground
  * CS - to digital pin 10  (SS pin)
  * SDI - to digital pin 11 (MOSI pin)
  * CLK - to digital pin 13 (SCK pin)
 
 created 10 Aug 2010 
 by Tom Igoe
 
 Thanks to Heather Dewey-Hagborg for the original tutorial, 2005
 
*/


// inslude the SPI library:
#include <SPI.h>


// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;

void setup() {
  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin(); 
  SPI.setDataMode(SPI_MODE0);
}

void loop() 
{
  delay(1000);
  //digitalPotWrite(0x0C, 0x01); //SHUT_DOWN
  digitalPotWrite(0xFF, 0x0F); //DISPLAY_TEST
  delay(1000);
  digitalPotWrite(0xFF, 0x00); //DISPLAY_TEST
  //digitalPotWrite(0x0C, 0x01); //SHUT_DOWN
  digitalPotWrite(0x09, 0x00); //DECODE_MODE
  digitalPotWrite(0x0B, 0x07); // scan limit
  digitalPotWrite(0x0A, 0xFF); // intensity
  digitalPotWrite(0x00, 0xFF);   //SEND DATA TO DIG0
  
  /*
  // go through the six channels of the digital pot:
  for (int channel = 0; channel < 6; channel++) 
  { 
    // change the resistance on this channel from min to max:
    for (int level = 0; level < 255; level++) 
    {
      digitalPotWrite(channel, level);
      delay(10);
    }
    // wait a second at the top:
    delay(100);
    // change the resistance on this channel from max to min:
    for (int level = 0; level < 255; level++) 
    {
      digitalPotWrite(channel, 255 - level);
      delay(10);
    }
  }
*/
}

int digitalPotWrite(int address, int value) 
{
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin,LOW);
  delay(500);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin,HIGH);
 delay(500); 
}
