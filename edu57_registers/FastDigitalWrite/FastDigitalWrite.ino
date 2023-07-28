/*
* Arduino Fast digitalWrite
* https://roboticsbackend.com/arduino-fast-digitalwrite/
*/

#include <avr/io.h>
#include <util/delay.h>

#define ledPin 13      // internal LED pin
#define interruptPin 2 // A button between PIN-2 and GND
#define TEST_PIN 8

// a variable can change inside an ISR, thus must be volatile:
volatile byte state = LOW;

byte pin13mask = (1 << 5);

#define D13_SET_OUTPUT      DDRB  |=  pin13mask // Set PIN D13 to OUTPUT
#define D13_SET_INPUT       DDRB  &= ~pin13mask // Set PIN D13 to INPUT
#define D13_WRITE_HIGH      PORTB |=  pin13mask // Set bit related to PIN D13 to HIGH,rest are untouched.
#define D13_WRITE_LOW       PORTB &= ~pin13mask // Set bit related to PIN D13 to LOW,rest are untouched.

void myISR_Blink()
{
    static unsigned long millis_prev;
    const int debounce_delay = 10; //100

    if (millis() - debounce_delay > millis_prev)
    {
        state = !state;   // reverse
    }

    millis_prev = millis();
}

void setup()
{
    Serial.begin(9600);
    // D13_SET_OUTPUT; // Set PIN 13 to OUTPUT
    pinMode(TEST_PIN, OUTPUT);

    unsigned long timeBegin = micros();

// #define USE_DIGITAL_WRITE
#define USE_REGISTER_WRITE

#ifdef USE_DIGITAL_WRITE
    for(int i = 0; i < 500; i++)
    {
        digitalWrite(TEST_PIN, HIGH);
        digitalWrite(TEST_PIN, LOW);
    }
    // Usual output:
    // Total duration: 3396 µs
    // Duration for one digitalWrite: 33.96 µs (microseconds)
#endif

#ifdef USE_REGISTER_WRITE
    for(int i = 0; i < 500; i++)
    {
        // this one is fastest:
        //PORTB = B00000001; // Set pin to 1
        //PORTB = B00000000; // Set pin to 0

        // bit slower:
        //PORTB |=   1 << TEST_PIN;  // Set pin to 1
        // PORTB &= ~(1 << TEST_PIN); // Set pin to 0

        PORTB = PORTB | B00000001; // Set pin to 1
        PORTB = PORTB & B11111110; // Set pin to 0
    }
    // Usual output:
    // Total duration: 192 µs
    // Duration for one digitalWrite: 0.19 µs (microseconds)
#endif

    unsigned long timeEnd = micros();
    unsigned long totalDuration = timeEnd - timeBegin;

    // Note: here we used micros() to get the time.
    // The micros() can only be precise up to 4 microseconds,
    // so that is another good reason to compute an average:
    double oneTimeDuration = (double)totalDuration / 1000.0;

    Serial.println("--------------------");

    Serial.print("Total duration: ");
    Serial.print(totalDuration);
    Serial.println(" µs");

    Serial.print("Duration for one digitalWrite: ");
    Serial.print(oneTimeDuration);
    Serial.println(" µs (microseconds)");
}


void loop()
{
    /*
    D13_WRITE_HIGH; // LED ON
    _delay_ms(300);
    // delay(300);
    D13_WRITE_LOW;  // LED OFF
    _delay_ms(300);
    // delay(300);
    */
}

