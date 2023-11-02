/*
* Example Timer Interrupt
  https://electronoobs.com/eng_arduino_tut140.php

 Why we use this prescalar then?
 Well imagine you want to make an LED blink each half second. So you want to trigger the timer interruption each 500 ms, right?

 The system clock is 16 MHz so each pulse is 62 nanoseconds. In order to count up to 500 ms, which are 500.000.000 ns we would need to count up to 800.000 so we can't use the 16 bit register for example, because it could only count up to 65536.

 But if we use a prescalar of 256, we would have a pulse each 16 microseconds.
 So now, to count up top 500 ms we would need 500.000 divided by 16 equal to 31250 pulses, and now we could use the 16-bits register.
*/


#define ledPinG 13
#define ledPinR 12

/*
Short explanation:
Timer1 call ISR every 10 ms using prescalar of 256.
Calculation: (for 0.01 s)
    System CLock 16 MHz and prescalar is 256;
    Timer1 speed = 16MHz/256 = 62.5kHz
    Pulse time = 1/62.5kHz = 16us (microseconds)
    Count up to 100ms:
    0.01 sec / 0.000016 sec = 625-1 (so this value the OCR Register must have)
    (16000000*0.01)/256 - 1 = 624
    10 ms is 100 Hz, so using frequency:
    (16000000/256/100) - 1 = 624

Timer1 call ISR every 100 ms using prescalar of 256.
Calculation: (for 0.1 s)
    System CLock 16 MHz and prescalar is 256;
    Timer1 speed = 16MHz/256 = 62.5kHz
    Pulse time = 1/62.5kHz = 16us (microseconds)
    Count up to 100ms:
    0.1 sec / 0.000016 sec = 6250-1 (so this value the OCR Register must have)
    (16000000*0.1)/256 - 1 = 6249
    100ms is 10 Hz, so using frequency:
    (16000000/256/10) - 1 = 6249

Timer1 call ISR every 500 ms using prescalar of 256.
Calculation: (for 0.5 s)
    System CLock 16 MHz and prescalar is 256;
    Timer1 speed = 16MHz/256 = 62.5kHz
    Pulse time = 1/62.5kHz = 16us (microseconds)
    Count up to 500ms:
    0.5 sec / 0.000016 sec = 31250-1 (so this value the OCR Register must have)
    (16000000*0.5)/256 - 1 = 31249
    500 ms is 2 Hz, so using frequency:
    (16000000/256*2) - 1 = 31249

Calculation: (for 1 s)
    System CLock 16 MHz and prescalar is 256;
    Timer1 speed = 16MHz/256 = 62.5kHz
    Pulse time = 1/62.5kHz = 16us (microseconds)
    Count up to 1000 ms:
    1 sec / 0.000016 sec = 62500-1 (so this value the OCR Register must have)
    (16000000*1)/256 - 1 = 62499
    1000 ms is 1 Hz, so using frequency:
    (16000000/256/1) - 1 = 62499
*/
int timer1_counter;
bool LED_STATE = true;

void setup()
{
    Serial.begin(9600);
    pinMode(ledPinG, OUTPUT);
    pinMode(ledPinR, OUTPUT);

    // initialize timer1:
    noInterrupts(); // disable all interrupts
    // cli(); // disable all interrupts
    /*1. First we reset the control register to make sure we start with everything disabled. */
    TCCR1A = 0; // Reset entire TCCR1A to 0
    TCCR1B = 0; // Reset entire TCCR1B to 0

    TCCR1B |= (1<<WGM12);    // reset on match (сброс по совпадению)

    /*2. We set the prescalar to the desired value by changing one of
    * the CS12, CS11 or CS10 bits.
    For 256: CS12=1 */

    // TCCR1B |= (1 << CS12); // 256 prescaler
    TCCR1B |= 0b00000100;     // 256 prescaler

    TIMSK1 |= 0b00000010;  //  allow interrups if OCIE1A match.

    // OCR1A = 6249; // Set compare register to this value for 100ms ISR
    // OCR1A = 31249; // Set compare register to this value for 500ms ISR
    OCR1A = 62499; // Set compare register to this value for 1s ISR

    TCNT1 = 0;

    interrupts(); // enable all interrupts
}

ISR (TIMER1_COMPA_vect)  // interrupt service routine, triggers every 500 ms.
{
    // TCNT1 = 0; // reset timer to 0 for next interrupt
                  // Note that if Reset on match set (WGM12), this is not needed.
    LED_STATE =!LED_STATE; // invert led state

    //digitalWrite(ledPinG, LED_STATE);
    // digitalWrite(ledPinR, !LED_STATE);
    Serial.println(millis());
}

void loop()
{
  digitalWrite(ledPinG, LED_STATE);
  //LED_STATE =!LED_STATE; // invert led state
  //delay(500);
}
