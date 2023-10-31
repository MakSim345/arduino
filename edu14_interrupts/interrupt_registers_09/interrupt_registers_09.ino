/*
Arduino: External interrupts
https://www.youtube.com/watch?v=PIX6TZxukyo
*/

#include <avr/io.h>
#include <util/delay.h>

#define ledPin 13      // internal LED pin
#define interruptPin 2 // A button between PIN-2 and GND

// a variable can change inside an ISR, thus must be volatile:
volatile byte state = LOW;


ISR(INT0_vect)
{
    // code that runs when external interrupt 0 is triggered:

    PORTD =^ 0b10000000; // toggle PIN7
    // this toggle is same as below:
    /*
    if (PIND & 0b00000100) // check if pin 2 is HIGH (BTN NOT pressed)
    {
        PORTD &= 0b01111111; // turn LED ON
    }
    else
    {
        PORTD &= 0b10000000; // turn LED OFF
    }
    */
}



int main()
{
    // setup code that runs only once
    DDRD = 0b10000000; // set PIN7 as output (for LED)
    PORTD = 0b10000100; // enable internal pullup on PIN2 (for BTN)

    // genarate an IRQ:
    EICRA = 0b00000001; // 01 means "any logical change" on INT0
    EIMSK = 0b10000001; // enable INT0
    sei(); // enable interrupt globally

    while(1)
    {
        // main endless loop
        //PORTB |= 1 << PORTB_5;    // Set pin to 1
        _delay_ms(500);
        // PORTB &= ~(1 << PORTB_5); // Set pin to 0
        //_delay_ms(100);
    }
    return 0;
}
