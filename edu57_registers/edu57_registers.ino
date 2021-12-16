/*
 * 
 * Port Register Control | Increase speed of Read/Write - Arduino101
 * https://www.youtube.com/watch?v=UhTRrjYXqPU&list=PLsR1AO4QH1AyR0HEQH9eq53K9RSfzXsbD&index=2
*/

#include <avr/io.h>
#include <util/delay.h>

#define ledPin 13      // internal LED pin
#define interruptPin 2 // A button between PIN-2 and GND

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

#define ARDUINO_IDE_IN_USE
#ifdef ARDUINO_IDE_IN_USE

void setup()
{
  Serial.begin(9600);
  // D13_SET_OUTPUT; // Set PIN 13 to OUTPUT
  pinMode(9, OUTPUT);
}

void loop()
{
  digitalWrite(9, HIGH);
  // delay(200);
  digitalWrite(9, LOW);
  delay(5);
}


void test_loop()
{
  D13_WRITE_HIGH; // LED ON
  //_delay_ms(300);
  delay(1000);
  D13_WRITE_LOW;  // LED OFF
  //_delay_ms(300);
  delay(1000);
}
#endif // ARDUINO_IDE_IN_USE

/******************************************************************/

// #define CPP_IN_USE
#ifdef  CPP_IN_USE

// PORTB pin 5 is UNO pin 13, internal LED (and SCK). Also it is 19 leg on the ATMEGA chip
#define PORTB_5   5

int main()
{
    DDRB |= 1 << PORTB_5;     // Set bit 5 of the DDRB to HIGH (output mode)

    while(1)
    {   // main endless loop
        PORTB |= 1 << PORTB_5;    // Set pin to 1
        _delay_ms(500);
        PORTB &= ~(1 << PORTB_5); // Set pin to 0
        _delay_ms(100);
    }
    return 0;
}
#endif // CPP_IN_USE

