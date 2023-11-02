/*
* Example Timer1 Overflow Interrupt
*/

#define ledPin 13
int timer1_counter;

void setup()
{
    pinMode(ledPin, OUTPUT);

    // initialize timer1
    noInterrupts();           // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;

    // Set timer1_counter to the correct start value for our interrupt interval
    // timer1_counter = 64886;   // preload timer 65536-16MHz/256/100Hz
    // timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
    // timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
    // timer1_counter = 40535;   // preload timer 65536-16MHz/64/10Hz
    /* Note:
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


    /* Example: We need 100 ms -> 0.1s -> 10 Hz.
     * Timer1 max value: 65536
     * Prescaler: 64
     * Formula: 65536-(16MHz*0.1s)/64 // note 100ms = 0.1
     * Formula: 65536-(16MHz/64/10Hz)
     * By writing 40535 to the TCNTx register, we guarantee that it's going to tick 25000 ticks
     * to reach the overflow state.
     * In the timer overflow interrupt ISR handler, we also need to preload the TCNTx register
     * with the same value and keep repeating over and over again.
     */
    // Preload timer use seconds: 65536-(16000000*0.1)/64
    // Preload timer use Hz:      65536-16000000/64/10
    timer1_counter = 40535;

    TCNT1 = timer1_counter;   // preload timer to init value, so it count to 65536 from it
    // TCCR1B |= (1 << CS12);    // 256 prescaler
    TCCR1B |= (1<<CS10)|(1<<CS11); // 64 prescaler
    TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
    interrupts();             // enable all interrupts
}

ISR(TIMER1_OVF_vect)        // interrupt service routine
{
    TCNT1 = timer1_counter;   // preload timer
    digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
}

void loop()
{
  // your program here...
}



