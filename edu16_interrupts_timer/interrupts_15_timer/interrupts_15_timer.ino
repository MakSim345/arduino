/*
* Example Timer1 Overflow Interrupt
https://www.youtube.com/watch?v=IdL0_ZJ7V2s&list=PLAROrg3NQn7cyu01HpOv5BWo217XWBZu0&index=22

Precise 1 second Overflow Interrupt
*/

#define ledPinG 13
#define ledPinR 12

int timer1_counter;
volatile int i = 0;
bool LED_STATE = true;

void setup()
{
    pinMode(ledPinG, OUTPUT);
    pinMode(ledPinR, OUTPUT);
    Serial.begin(9600);

    // initialize timer1
    noInterrupts(); // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;
     /* Note:
     * 1Hz   - 1000ms(1 s)
     * 2Hz   - 500ms(0.5s)
     * 10Hz  - 100ms(0.1s)
     * 50Hz  - 20ms (0.02s)
     * 100Hz - 10ms (0.01s)
     * 1000Hz- 1ms (0.001s)
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

    // TCCR1B |= (1<<CS12); // 256 prescaler
    // Note: prescaler 256, start from 0;
    // The IRQ will occure every 1.048 seconds.

    TCCR1B |= (1<<CS12); // 256 prescaler
     /* Example: We need 1 sec -> 1 Hz.
     * Timer1 max value: 65536
     * Prescaler: 256
     * Preload timer use seconds: 65536-(16MHz*1s)/256 // note 1000ms = 1
     * Preload timer use Hz:    : 65536-(16MHz/256/1Hz)
     * By writing 3036 to the TCNTx register, we guarantee that it's going to tick 25000 ticks
     * to reach the overflow state.
     * In the timer overflow interrupt ISR handler, we also need to preload the TCNTx register
     * with the same value and keep repeating over and over again.
     */
    timer1_counter = 3036; // preload timer
    TCNT1 = timer1_counter;// preload timer to init value, so it count to 65536 from it

    TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
    interrupts();             // enable all interrupts
}

ISR(TIMER1_OVF_vect)        // interrupt service routine
{
    TCNT1 = timer1_counter;   // preload timer
    //digitalWrite(ledPinG, digitalRead(ledPinG) ^ 1);
    //digitalWrite(ledPinR, digitalRead(ledPinR) ^ 1);
    LED_STATE =!LED_STATE; // invert led state

    digitalWrite(ledPinG, LED_STATE);
    digitalWrite(ledPinR, !LED_STATE);

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

