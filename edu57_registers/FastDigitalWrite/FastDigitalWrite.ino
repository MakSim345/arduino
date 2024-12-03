/*
* Arduino Fast digitalWrite
* https://roboticsbackend.com/arduino-fast-digitalwrite/
*/

#include <avr/io.h>
#include <util/delay.h>

#define ledPin 13      // internal LED pin
#define interruptPin 2 // A button between PIN-2 and GND
#define TEST_PIN 9

// a variable can change inside an ISR, thus must be volatile:
volatile byte state = LOW;

byte pin13mask = (1 << 5);

// Define the bitmask for pin 9 (PB1)
byte pin09mask = (1 << 1); // This sets the 1st bit (pin 9)

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

// #define TEST_IN_SETUP
#define USE_DIGITAL_WRITE
//#define USE_REGISTER_WRITE


#ifdef TEST_IN_SETUP

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
#endif
    Serial.println("--------------------");
}


void loop()
{
    /*
    digitalWrite(TEST_PIN, HIGH);
    digitalWrite(TEST_PIN, LOW);
    delay (3);
    */


    //PORTB |= B00000010; // Set the second bit of PORTB to HIGH
    //PORTB = B00000010;
    //PORTB = B00000000;
    PORTB |=  (1 << 1); // Set bit related to PIN D09 to HIGH.
    PORTB &= ~(1 << 1); // Set bit related to PIN D09 to LOW.

    delay(3);
}
