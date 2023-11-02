/*
* Example Timer1 Overflow Interrupt
https://www.youtube.com/watch?v=U194XOYCs8Q
Using ONLY Prescaler, no init TCNT1!
*/

#define ledPin 12
// int timer1_counter;
volatile int i = 0;

void setup()
{
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);

    // initialize timer1
    noInterrupts(); // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    /*
    For the 16bit Timer1 clock you have overflows of:
    0 ---- 4.096ms
    8 ---- 32.77ms
    64 --- 262.14ms
    256 -- 1.048s
    1024 - 4.19s
    */

    /* prescaler:
     * TCCR1B |= (1<<CS11);           // CLK/8
     * TCCR1B |= (1<<CS10)|(1<<CS11); // CLK/64
     * TCCR1B |= (1<<CS12);           // CLK/256
     * TCCR1B |= (1<<CS10)|(1<<CS12); // CLK/1024
    */

    // TCCR1B |= (1<<CS10)|(1<<CS12); // 1024 prescaler
    // Note: prescaler 1024, start from 0;
    // The IRQ will occure every 4 seconds.

    TCCR1B |= (1<<CS12); // 256 prescaler
    // Note: prescaler 256, start from 0;
    // The IRQ will occure every 1.048 seconds.

    TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
    interrupts();             // enable all interrupts
}

ISR(TIMER1_OVF_vect)        // interrupt service routine
{
    digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
    i++;
    if (i == 100)
    {
        i = 0;
    }

    Serial.println(millis());
}

void loop()
{
  static int toShow = 0;
  if (i != toShow)
  {
    toShow = i;
    Serial.println(toShow);
    //Serial.print("Time: ");
    //Serial.println(micros());

  }
  //unsigned long timeEnd = micros();
  //unsigned long timeDuration = timeEnd - timeBegin;
  //double averageDuration = (double)timeDuration / 1000.0;

  delay(100);
}
