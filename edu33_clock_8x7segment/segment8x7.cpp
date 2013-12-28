#include "segment8x7.h"

segment8x7::segment8x7(void)
{
    init_segments();
    init_digits();

    pinMode(LATCH, OUTPUT);
    pinMode(CLOCK, OUTPUT);
    pinMode(DATA, OUTPUT);
    digitalWrite(LATCH, HIGH);
    delay(1000);
}

void segment8x7::init_segments()
{    
    // array to activate particular digit on the 8x7segment module
    // it is the common anode of 7 segment
    anode[0] = 0b10000000;  //digit 1 from right
    anode[1] = 0b01000000;  //digit 2 from right
    anode[2] = 0b00100000;  //digit 3 from right
    anode[3] = 0b00010000;  //digit 4 from right
    anode[4] = 0b00001000;  //digit 5 from right
    anode[5] = 0b00000100;  //digit 6 from right
    anode[6] = 0b00000010;  //digit 7 from right
    anode[7] = 0b00000001;  //digit 8 from right                                         
}

void segment8x7::init_digits()
{    
    //array for decimal number, it is the cathode, please refer to the datasheet.
    //therefore a logic low will activate the particular segment
                //xxPGFEDCBA, segment on 7 segment, P is the dot
    cathode[0] = 0b11000000;  // 0
    cathode[1] = 0b11111001;  // 1
    cathode[2] = 0b10100100;  // 2
    cathode[3] = 0b10110000;  // 3
    cathode[4] = 0b10011001;  // 4
    cathode[5] = 0b10010010;  // 5
    cathode[6] = 0b10000010;  // 6
    cathode[7] = 0b11111000;  // 7
    cathode[8] = 0b10000000;  // 8
    cathode[9] = 0b10010000;  // 9  
    cathode[10] = 0b01111111;  //dot                  
    cathode[11] = 0b11111111;  //blank
};     
 
void segment8x7::show_number(unsigned long value)
{
    displayNumber8x7segment(RIGHT, value);
}

void segment8x7::show_reverce_number(unsigned long value)    
{
    displayNumber8x7segment(LEFT, value);
}

//fucntion to send the serial data out to two 74HC595 serial to parallel shift register and activate the 7 segment.                  
void segment8x7::display8x7segment(byte digit, byte number)
{
    digitalWrite(LATCH, LOW);
    shiftOut(DATA, CLOCK, MSBFIRST, digit); 
    shiftOut(DATA, CLOCK, MSBFIRST, number);
    digitalWrite(LATCH, HIGH);  
}

void segment8x7::show_hour(int val)
{
    byte decimal[8] = {0};
    decimal[1] = val / 10;
    decimal[0] = val % 10;
  
    this->display8x7segment(0b00000001, cathode[decimal[1]]);
    this->display8x7segment(0b00000010, cathode[decimal[0]]);
}

void segment8x7::show_min(int val)
{
    byte decimal[8] = {0};
    decimal[1] = val / 10;
    decimal[0] = val % 10;
  
    this->display8x7segment(0b00001000, cathode[decimal[1]]);
    this->display8x7segment(0b00010000, cathode[decimal[0]]);
}

void segment8x7::show_sec(int val)
{
    byte decimal[8] = {0};
    decimal[1] = val / 10;
    decimal[0] = val % 10;
  
    this->display8x7segment(0b01000000, cathode[decimal[1]]);
    this->display8x7segment(0b10000000, cathode[decimal[0]]);
}
//function to display value on 8x7 segment display according to the justify state
void segment8x7::displayNumber8x7segment(byte justify, unsigned long value)
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
   
   if (justify == RIGHT)
   {   
     for(byte e = 8; e > 0 ; e --)
     {
       if(zero == 0)
       {
         if(decimal[e-1] != 0)          
         {
           display8x7segment(anode[e-1], cathode[decimal[e-1]]);
           zero = 1;     
         }
       }
       else display8x7segment(anode[e-1], cathode[decimal[e-1]]);
     
     delay(MultiplexDelay); 
     }
   }
   else  //if justify == left
   { 
     byte d = 0;     
     for(byte e = 8; e > 0; e--)
     {       
       if(zero == 0)
       {
         if(decimal[e-1] != 0)         
         {
           display8x7segment(anode[7], cathode[decimal[e-1]]);
           zero = 1;
           d ++;     
           delay(MultiplexDelay); 
         }
       }
       else 
       {
         display8x7segment(anode[7-d], cathode[decimal[e-1]]);
         d ++;
         delay(MultiplexDelay); 
       }     
     }
   }
}
