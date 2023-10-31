/*
Pin Change Interruptions ISR | PCINT
"https://youtu.be/ZDtRWmBMCmw"

NOTE: does not compile for Leonardo!
Error: "'PCMSK1' was not declared in this scope"

Note for PCIE2:
Any change on any enabled PCINT[23:16] pin will cause an interrupt
This is unlike attachInterrupt() function,
where we can choose RISING, FALL etc.
*/

#define BUTTON_04 4
#define BUTTON_05 5
#define BUTTON_13 13

volatile int state = LOW;
volatile int setpoint = 0;

/*
 * ISR for Port D
*/
ISR(PCINT2_vect)
{
  // code that runs when pin change interrupt PORT D is triggered
  if (PIND & 0b00010000) // D4 is HIGH
  {
    setpoint--;
  }

  if (PIND & 0b00100000) // D5 is HIGH
  {
    setpoint++;
  }
}

void setup()
{
  // setup code that only runs once:
  //DDRD =  0b10000000; // set pin 7 as output (Register D, PD7, PCINT23)
  //PORTD = 0b11100000; // enable pullup on pins 5,6,7 - Port D

  PCICR  |= 0b00000100; // enable PORT D for PCI
  PCMSK2 |= 0b00110000; // So pins D4 and D5 can triggered interrupt

  // init pushbuttons:
  pinMode(BUTTON_04, INPUT_PULLUP);
  pinMode(BUTTON_05, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop()
{
  // do main loop here.
  char out_buf[255];  //reserve the string space first
  //digitalWrite(BLUELED, LOW);
  //digitalWrite(REDLED, LOW);
  //digitalWrite(WHITELED, LOW);
  //digitalWrite(GREENLED, HIGH); delay(1000);
  //digitalWrite(GREENLED, LOW); delay(1000);
  delay(1000);
  sprintf(out_buf, "setpoint - %d", setpoint);
  Serial.println(out_buf);    // print the info

}
// Serial.write("press -- \n"); //
