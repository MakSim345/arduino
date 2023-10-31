/*
Pin Change Interrupts in Arduino Nano
"https://www.youtube.com/watch?v=3vjpapdfjwA"

NOTE: does not compile for Leonardo!
Error: "'PCMSK1' was not declared in this scope"
*/


#define GREENLED 12
#define BLUELED  10
#define REDLED   8
#define WHITELED 6

#define BUTTON_04 4
#define BUTTON_13 13

#define TIME_CHATTER 5

volatile int state = LOW;

void setup()
{
  pinMode(BUTTON_04, INPUT);       // set button to input
  pinMode(BUTTON_13, INPUT);       // set button to input

  Serial.begin(9600);           // init serial

  // Enable PCI
  PCICR  |= 0b00000001; // enable PORT B for PCI
  PCMSK0 |= 0b00100000; // enable INT5 attached to pin D13 (SW1)

  PCICR  |= 0b00000010; // enable PORT C for PCI
  PCMSK1 |= 0b00100000; // enable INT13 attached to pin A5 (SW2)

  PCICR  |= 0b00000100; // enable PORT D for PCI
  PCMSK2 |= 0b00010000; // enable INT20 attached to pin D4 (Sensor)
}

void loop()
{
  // do main loop here.
  digitalWrite(BLUELED, LOW);
  digitalWrite(REDLED, LOW);
  digitalWrite(WHITELED, LOW);
  digitalWrite(GREENLED, HIGH); delay(1000);
  digitalWrite(GREENLED, LOW); delay(1000);

  // Serial.write("press -- \n"); //
}

/*
 * ISR for INT5 (SW1 on pin D13)
*/
ISR(PCINT0_vect)
{
  for(int i=1; i<=50; i++)
  {
     digitalWrite(BLUELED, HIGH); delayMicroseconds(16000);
     digitalWrite(BLUELED, LOW);  delayMicroseconds(16000);
  }
}

/*
 * ISR for INT13 (SW2 on pin A5)
*/
ISR(PCINT1_vect)
{ for(int i=1; i<=50; i++)
  {
     digitalWrite(BLUELED, HIGH); delayMicroseconds(16000);
     digitalWrite(BLUELED, LOW);  delayMicroseconds(16000);
   }
}

/*
 * ISR for INT20 (sensor) on pin D4)
*/
ISR(PCINT2_vect)
{ for(int i=1; i<=50; i++)
  {
     digitalWrite(BLUELED, HIGH); delayMicroseconds(16000);
     digitalWrite(BLUELED, LOW);  delayMicroseconds(16000);
   }
}


/*
void loop_bkp()
{
  val = digitalRead(BUTTON);    // got a button state
  if(val==HIGH)                 // it is pressed
  {
    digitalWrite(LED, HIGH); // led on
    //Serial.println("H");
  }
  else                          // button released
  {
    digitalWrite(LED, LOW);  // led OFF
    //Serial.println("L");
  }
  delay(100);
}

*/
