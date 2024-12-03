#ifdef YS_TEST_1009929992
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
 */


#define TRACE_LINE Serial.write
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int ledPin = 13;
int ledGreenPin = 1;
int longWait = 500;
int shortWait = 200;
long randOn = 0;                  // Initialize a variable for the ON time
long randOff = 0;                 // Initialize a variable for the OFF time
int bitsToSend = 0;
int whichPin=0;
int  whichState = 0;

void dispBitsToArr(unsigned Val, char* _array);
char _array_bit[9] = {'0','0','0','0','0','0','0','0','\0'};

// the setup routine runs once when you press reset:
void setup()
{
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  randomSeed (analogRead (0));    // randomize
  Serial.begin(9600);
}

void random_leds()
{

  randOn = random (100, 1200);    // generate ON time between 0.1 and 1.2 seconds
  randOff = random (200, 900);    // generate OFF time between 0.2 and 0.9 seconds

  bitWrite(bitsToSend, 4, 1);
  dispBitsToArr(bitsToSend, _array_bit);
  TRACE_LINE("status bits:");
  TRACE_LINE(_array_bit);

  Serial.write("--\n"); //
  char v_str[8] = "       ";  //reserve the string space first
  itoa(randOn, v_str, 6);
  // lcd.printIn(v_str);
  int bytesSent = Serial.write(v_str); //send the string and return the length of the string.
  Serial.write("--\n");

  digitalWrite(ledPin, HIGH);   // sets the LED on
  digitalWrite(ledGreenPin, LOW);
  delay(randOn);                // waits for a random time while ON

  digitalWrite(ledPin, LOW);    // sets the LED off
  digitalWrite(ledGreenPin, HIGH);
  delay(randOff);               // waits for a random time while OFF
}

// the loop routine runs over and over again forever:
void loop()
{
  // Serial.write("App start.\n");
  random_leds();
/*
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(longWait);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(longWait);               // wait for a second
  */

  /*
  digitalWrite(led, HIGH);
  delay(longWait);

  digitalWrite(led, LOW);
  delay(longWait);
  digitalWrite(led, HIGH);
  delay(longWait);
  digitalWrite(led, LOW);
  delay(longWait);
  digitalWrite(led, HIGH);
  delay(shortWait);
  digitalWrite(led, LOW);
  delay(shortWait);
  digitalWrite(led, HIGH);
  delay(shortWait);
  digitalWrite(led, LOW);
  delay(shortWait);
  digitalWrite(led, HIGH);
  delay(shortWait);
  digitalWrite(led, LOW);
  delay(longWait);
  */
}
void dispBitsToArr(unsigned Val, char* _array)
{
    unsigned BIT = 7;
    //unsigned BIT = 15;
    //unsigned   BIT = 30;
    unsigned   c;
    unsigned   dispMask=1<<BIT;
    int        nBin;
    int nPrintRes = 1;

    //if (nPrintRes)
    //   fprintf (fi, "Val = %d(0x%X)\n", Val, Val);

    for (c = 1; c<=BIT+1; c++)
    {
        nBin = Val & dispMask ? '1' : '0' ;
        _array[c-1] = nBin;
        Val <<= 1;

        if ((c%9 == 0) && nPrintRes)
        {
            _array[c-1] = ' ';
        }
    }
}

#endif
