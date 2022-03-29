/*
 * 
 * Bare-Metal MCU #2 - Set Registers by Memory Address
 * https://www.youtube.com/watch?v=W8REqKlGzDY&list=PLNyfXcjhOAwOF-7S-ZoW2wuQ6Y-4hfjMR&index=3
*/

const uint8_t btn_pin = 2;
const uint8_t led_pin = 5;

// Globals
const uint8_t led_state = LOW;
const uint8_t btn_prev  = HIGH;

void setup()
{
	//pinMode(btn_pin, INPUT_PULLUP);
	//pinMode(led_pin, OUTPUT);
    DDRB = 32;
}

void loop()
{
	volatile byte* mitch = 0x25;
    *mitch = 32; // PORTB = 0b00010000;
    // PORTD |= 0b10000000;
	delay(500);

    *mitch = 0; // PORTB = 0b00000000;
	// PORTD &= 0b01111111;
	delay(500);
}

