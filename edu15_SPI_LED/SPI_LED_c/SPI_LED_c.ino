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
 
*/

/*
*********************************************************************************************************
* COSTANTS FOR MAX7219
*********************************************************************************************************
*/
#define REG_DECODE        0x09                        // "decode mode" register
#define REG_INTENSITY     0x0a                        // "intensity" register
#define REG_SCAN_LIMIT    0x0b                        // "scan limit" register
#define REG_SHUTDOWN      0x0c                        // "shutdown" register
#define REG_DISPLAY_TEST  0x0f                        // "display test" register

#define INTENSITY_MIN     0x00                        // minimum display intensity
#define INTENSITY_MAX     0x0f                        // maximum display intensity


// inslude the SPI library:
#include <SPI.h>

// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;
const int ledGreenPin = 8;
bool  first_time = true;
int rand_val_1 = 0;
int rand_val_2 = 0;
int rand_val_3 = 0;
int rand_val_4 = 0;

void spi_control_led()
{
    //Imposto scan quattro cifre digit 0,1,2,3
    max7219_Write(REG_SCAN_LIMIT, 3);                
 
    //Imposto luminosita massima
    max7219_Write(REG_INTENSITY, INTENSITY_MAX);                
 
    // Imposto code B su 4 digit
    max7219_Write(REG_DECODE, 0x0F);
   
    //write numero 9865:
    max7219_Write(3, 9);  
    max7219_Write(2, 8);   
    max7219_Write(1, 6);   
    max7219_Write(0, 5);  
}

void led_display_test()
{
    // performs one time blink
     max7219_Write(REG_DISPLAY_TEST, 0x01);  
     //max7219_Write(0xFF, 0x0F); //DISPLAY_TEST
     delay(500);
     max7219_Write(REG_DISPLAY_TEST, 0x00);  
}
void led_display_normal_init()
{
    max7219_Write(0x0C, 0x01); //SHUT_DOWN Register. shutdown mode if 0. Else it is normal mode.
    max7219_Write(0x09, 0xFF); //DECODE_MODE
    max7219_Write(0x0B, 0x07); // scan limit
    max7219_Write(0x0A, 0x08); // intensity
}

void setup() 
{
     // set the slaveSelectPin as an output:
    pinMode (slaveSelectPin, OUTPUT);
    pinMode(ledGreenPin, OUTPUT); 
    randomSeed (analogRead (0));    // randomize 
    // initialize SPI:
    SPI.begin(); 
    SPI.setDataMode(SPI_MODE0);
}

void loop() 
{
    //spi_control_led();
    if (first_time == true)
    {
        led_display_test();      
        led_display_normal_init();
        first_time = false;
    }
    
    rand_val_1 = random (0, 9);
    rand_val_2 = random (0, 9);
    rand_val_3 = random (0, 9);
    rand_val_4 = random (0, 9);

    digitalWrite(ledGreenPin, LOW);
    delay(500);
    //max7219_Write(0xFF, 0x00); //DISPLAY_TEST
    digitalWrite(ledGreenPin, HIGH);
   
    // max7219_Write(0x01, 0x02);   //SEND DATA TO DIG0
     //write numero 9865:
    //max7219_Write(7, rand_val_4);
    //max7219_Write(6, 0);
    
    max7219_Write(6, 0xD);
    max7219_Write(5, 0xD);
    max7219_Write(4, 0xB);
    max7219_Write(3, 0xC);
    max7219_Write(2, rand_val_2);
    max7219_Write(1, rand_val_1);
  
    //max7219_Write(0x0C, 0x01); //SHUT_DOWN
    delay(100);
}

int max7219_Write(int address, int value) 
{
    // take the SS pin low to select the chip:
    digitalWrite(slaveSelectPin, LOW);
    //delay(200);
    //  send in the address and value via SPI:
    SPI.transfer(address);
    SPI.transfer(value);
    // take the SS pin high to de-select the chip:
    digitalWrite(slaveSelectPin, HIGH);
    //delay(200); 
}
