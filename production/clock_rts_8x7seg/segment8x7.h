#ifndef segment8x7_h
#define segment8x7_h

/*
  segment8x7 - Library for 8 seven-segments LED indicators.
 
 Connections 8x7 segment module to Arduino
 * VCC to 5V
 * GND to GND
 * DIO to D10
 * SCK to D11
 * RCK to D12
 */

#include <Arduino.h>
#define LATCH  12  //pin 12 of Arduino connect to RCK of 8x7segment module 
#define CLOCK  11  //pin 11 of Arduino connect to SCK of 8x7segment module 
#define DATA   10  //pin 10 of Arduino connect to DIO of 8x7segment module 
#define MultiplexDelay  1  //delay for multiplexing between digit on 8x7segment module
#define LEFT   0  // define the value for left justify
#define RIGHT  1  // define the value for right justify
#define BLANK 11  //array element to make 7segment blank
#define DIGIT_ON  LOW
#define DIGIT_OFF HIGH
#define ERASE_SEGMENTS 10

class segment8x7
{
  public:
    segment8x7(void);
    void show_number(unsigned long number_to_show);
    void show_reverce_number(unsigned long number_to_show);
    void display8x7segment(byte digit, byte number);
    void show_hour(int);
    void show_min(int);
    void show_sec(int);

  private:
    void writeNumber(int num);
    void init_segments();
    void init_digits();

    
    void displayNumber8x7segment(byte justify, unsigned long value);
    byte cathode[12];
    byte anode[8];
};

#endif
