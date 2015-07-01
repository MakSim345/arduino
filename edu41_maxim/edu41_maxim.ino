//We always have to include the library
#include "LedControl.h"

/*
Now we need a LedControl to work with.
***** These pin numbers will probably not work with your hardware *****
pin 12 is connected to the DataIn
pin 11 is connected to the CLK
pin 10 is connected to LOAD
We have only a single MAX72XX.
*/
LedControl lc=LedControl(12, 11, 10, 1);

#define DEVICE 0

/* we always wait a bit between updates of the display */
unsigned long delaytime=250;
unsigned long _ctr = 110980;

void setup()
{
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  int _device = DEVICE;
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(_device, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(_device, 10);
  /* and clear the display */
  lc.clearDisplay(_device);
}


/*
  This method will scroll all the hexa-decimal
numbers and letters on the display. You will need at least
four 7-Segment digits. otherwise it won't really look that good.
*/
void scrollDigits()
{
  int _num = DEVICE;
  for(int i=0; i<13; i++)
  {
    lc.setDigit(_num, 8, i+1,   false);
    lc.setDigit(_num, 7, i+2,   false);
    lc.setDigit(_num, 6, i+3,   false);
    lc.setDigit(_num, 5, i+4,   false);
    lc.setDigit(_num, 4, i+5,   false);
    lc.setDigit(_num, 3, i+6,   false);
    lc.setDigit(_num, 2, i+7, false);
    lc.setDigit(_num, 1, i+8, false);
    lc.setDigit(_num, 0, i+9, false);
    delay(delaytime);
  }

  lc.clearDisplay(0);
  delay(delaytime);
}

void displayNumber(unsigned long value)
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
   int _num = 0;

   for(int i=0; i<8; i++)
   {
     lc.setDigit(0, i, decimal[i], false);
   }
}

void loop()
{
  //displayNumber(_ctr);
  //delay(delaytime);
  //_ctr = _ctr + 1;
  /*
  for (int a=0; a<8; a++)
  {
    lc.setDigit(0,a,a,true);
    delay(100);
  }
  
  for (int a=0; a<8; a++)
  {
    lc.setDigit(0,a,8,1);
    delay(100);
  }

  for (int a=0; a<8; a++)
  {
    lc.setDigit(0,a,0,false);
    delay(100);
  }
*/
  for (int a=0; a<8; a++)
  {
    lc.setChar(0,a,' ',false);
    delay(100);
  }
  for (int a=0; a<8; a++)
  {
    lc.setChar(0,a,'-',false);
    delay(100);
  }
  /*
  for (int a=0; a<8; a++)
  {
    lc.setChar(0,a,' ',false);
    delay(100);
  }*/
  //writeArduinoOn7Segment();
  //scrollDigits();
}
