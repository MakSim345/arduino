/*
* Example Timer1 Interrupt
* Flash LED every second

http://www.hobbytronics.co.uk/arduino-timer-interrupts

Arduino Timer Interrupts

When you want things to happen at a regular time interval, it can be easy to go for the delay() function.
But this just pauses the program for a specific time period which is wasteful especially if you need to
do other processing in the meantime.
This is where timers and interrupts come in to play.
The Arduino UNO has three timers
    Timer0 - An 8 bit timer used by Arduino functions delay(), millis() and micros().
    Timer1 - A 16 bit timer used by the Servo() library
    Timer2 - An 8 bit timer used by the Tone() library
The Mega boards have Timers 3,4,5 which may be used instead
In the example that follows, we shall use Timer1 for our interrupt. Obviously, if you are using the
Servo Library there will be a conflict, so you should choose another timer.
Here is the basics of an interrupt driven program. It is the basic LED flash program, but now instead of
using delays it uses an interrupt every half second to turn the LED on and then off, thus making the
LED flash once per second
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

    // Set timer1_counter to the correct value for our interrupt interval
    //timer1_counter = 64886;  // preload timer: 65536-16MHz/256/100Hz
    //timer1_counter = 64286;  // preload timer: 65536-16MHz/256/50Hz
    timer1_counter = 34286;    // preload timer: 65536-16MHz/256/2Hz

    TCNT1 = timer1_counter;   // preload timer
    TCCR1B |= (1 << CS12);    // 256 prescaler
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



