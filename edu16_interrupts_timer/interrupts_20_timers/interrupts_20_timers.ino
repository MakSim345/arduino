/*
From:
"https://www.youtube.com/watch?v=n6a8xbgfq1k&list=PLNAsgvPRQbqjgTS3gGTR-AsnhY3XhfOWa&index=12"
LabRazum, RUS
*/

// volatile means it can be changed anywhere
volatile unsigned long int timerLED1;
volatile unsigned long int timerLED1Sec;

#define MILLISECONDS_DELAY 500

unsigned long int timePreview =0; // counter?
bool LEDOn = 0; // keep the LED state

ISR (TIMER0_COMPA_vect)
{
    // method called by timer-conuter T0.
    timerLED1++; // incrementing every millisecond
    if(timerLED1 >= MILLISECONDS_DELAY)
    {
        timerLED1Sec++;
        timerLED1 = 0; //reset millisecond counter
        // Serial.write(timerLED1);
    }

    // if(timerLED1 - timePreview >= 1000)  //check if 1000 millisecond passed.
    if(timerLED1Sec - timePreview >= 1)  //check if 1 second passed.
    {
        LEDOn = !LEDOn;
        // timePreview = timerLED1; // update to current time
        timePreview = timerLED1Sec; // update to current time
        /*** here we can make any action, like det the Flag ***/
        digitalWrite(13, LEDOn);
    }
}


/*
Short explanation:
Timer0 call ISR every 1 ms using prescalar of 64.
Calculation: (for 0.001s)
    System CLock 16 MHz and prescalar 64;
    Timer0 speed = 16MHz/64 = 250kHz
    Pulse time = 0.001/250kHz = 4us (microseconds)

    Count up to 0.001 / 0.000004 = 250-1 (this value the OCR Register must have)
    1ms is 1000 Hz, so using frequency:
    (16000000/64*1000) - 1 = 249 (in hex: 0xF9)
*/

void setup()
{
    pinMode(13, OUTPUT); // pin 13 as an output
    /* set timer interrupt every 0.001 seconds
     * which will call TIMER0_COMPA_vect()
    */
    cli(); // disable all interrupts

    TCCR0A |= (1 << WGM01); // reset on match
    TCCR0B |= (1 << CS01) | (1 << CS00); // Prescaler to 64
    TIMSK0 |= (1 << OCIE0A); // allow interrups if match Register A

    OCR0A = 0xF9; // Set compare register to 249 for 1 ms IRQ)
    sei(); // allow interrupts

    Serial.begin(9600);
    Serial.println("Arduino: Setup OK. Running...");
}


void loop()
{
  // your program here...
}

//#define YS_AXIS_2021
#ifdef YS_AXIS_2021

/*
* Example Timer1 Interrupt

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
void test_setup()
{
//------ Timer0 ----------
TCCR0A = (1<<WGM01);   // Режим CTC (сброс по совпадению)
TCCR0B = (1<<CS00);    // Тактирование от CLK.
                       // Если нужен предделитель :
// TCCR0B = (1<<CS01);           // CLK/8
// TCCR0B = (1<<CS00)|(1<<CS01); // CLK/64
// TCCR0B = (1<<CS02);           // CLK/256
// TCCR0B = (1<<CS00)|(1<<CS02); // CLK/1024

OCR0A = 123;            // Верхняя граница счета. Диапазон от 0 до 255.
                        // Частота прерываний будет = Fclk/(N*(1+OCR0A))
                        // где N - коэф. предделителя (1, 8, 64, 256 или 1024)
TIMSK0 = (1<<OCIE0A);   // Разрешить прерывание по совпадению

//------ Timer1 ----------
TCCR1B = (1<<WGM12);    // Режим CTC (сброс по совпадению)
TCCR1B |= (1<<CS10);    // Тактирование от CLK.
                        // Если нужен предделитель :
// TCCR1B |= (1<<CS11);           // CLK/8
// TCCR1B |= (1<<CS10)|(1<<CS11); // CLK/64
// TCCR1B |= (1<<CS12);           // CLK/256
// TCCR1B |= (1<<CS10)|(1<<CS12); // CLK/1024

OCR1A = 2678;           // Верхняя граница счета. Диапазон от 0 до 65535.
                        // Частота прерываний будет = Fclk/(N*(1+OCR1A))
                        // где N - коэф. предделителя (1, 8, 64, 256 или 1024)
TIMSK1 = (1<<OCIE1A);   // Разрешить прерывание по совпадению

//------ Timer2 ----------
TCCR2A = (1<<WGM21);    // Режим CTC (сброс по совпадению)
TCCR2B = (1<<CS20);     // Тактирование от CLK.
                        // Если нужен предделитель :
// TCCR2B = (1<<CS21);                     // CLK/8
// TCCR2B = (1<<CS20)|(1<<CS21);           // CLK/32
// TCCR2B = (1<<CS22);                     // CLK/64
// TCCR2B = (1<<CS20)|(1<<CS22);           // CLK/128
// TCCR2B = (1<<CS21)|(1<<CS22);           // CLK/256
// TCCR2B = (1<<CS20)|(1<<CS21)|(1<<CS22); // CLK/1024

OCR2A = 234;            // Верхняя граница счета. Диапазон от 0 до 255.
                        // Частота прерываний будет = Fclk/(N*(1+OCR2A))
                        // где N - коэф. предделителя (1, 8, 32, 64, 128, 256 или 1024)
TIMSK2 = (1<<OCIE2A);   // Разрешить прерывание по совпадению

sei ();                 // Глобально разрешить прерывания
}

ISR (TIMER1_COMPA_vect)
{
    // Обработчик прерывания таймера 1
}

ISR (TIMER2_COMPA_vect)
{
    // Обработчик прерывания таймера 2
}

#endif
