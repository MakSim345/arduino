/*
 * LED with button
 * the led on PIN 13 will change it's state
 * after state of digital pin 2 has been changed
 */

int ledPin = 13;                // LED
int btnPin = 2;                // button
int val=0;
int interruptPin = 0;           // Interrupt 0 is on DIGITAL PIN 2!

volatile int state = LOW;

void setup()
{
  pinMode(ledPin, OUTPUT);      // set led to output
  // pinMode(btnPin, INPUT);       // set button to input

  //attachInterrupt(interruptPin, blink, CHANGE); // link interrupt 0 to function blink().
  attachInterrupt(interruptPin, stateChange, CHANGE); // link interrupt 0 to function blink().
  Serial.begin(9600);           // init serial
}

void loop()
{
  // do main loop here.
  digitalWrite(ledPin, state);          // show state
}

// ISR function
void stateChange()
{
  //noInterrupts();           // disable all interrupts
  state = !state;
  // Serial.write("press -- \n"); //
  //digitalWrite(ledPin, state);
  //interrupts();
}

// ISR function
void blink()
{
    static unsigned long millis_prev;
    if(millis() - 100 > millis_prev)
        state = !state;   // reverse

    millis_prev = millis();
}

/*
void loop()
{
  val = digitalRead(btnPin);    // got a button state
  if(val==HIGH)                 // it is pressed
  {
    digitalWrite(ledPin, HIGH); // led on
    //Serial.println("H");
  }
  else                          // button released
  {
    digitalWrite(ledPin, LOW);  // led OFF
    //Serial.println("L");
  }
  delay(100);
}

*/
