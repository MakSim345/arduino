/**
The main:

This code should work on the Arduino Leonardo and allow to use all available GPIO pins to create a keyboard keystrokes simulator.

1. The PCICR register is only enabled for the PCIE0 group:
    - as the Arduino Leonardo does not have the PCIE1 and PCIE2 groups.
2. The PCMSK0 register is used to enable the pin change interrupts for all the desired pins.
3. The Interrupt Service Routine (ISR) is defined for the PCINT0_vect:
    - instead of the PCINT2_vect, PCINT1_vect, and PCINT0_vect.


*
*/

#include "Keyboard.h"

// Define the pins for the pushbuttons
const int pin2 = 2;  // PCINT2
const int pin3 = 3;  // PCINT3
const int pin4 = 4;  // PCINT4
const int pin5 = 5;  // PCINT5
const int pin6 = 6;  // PCINT6
const int pin7 = 7;  // PCINT7
const int pin8 = 8;  // PCINT0
const int pin9 = 9;  // PCINT1
const int pin10 = 10;
const int pin11 = 11;
const int pin12 = 12;
const int pin13 = 13;
const int pinA0 = A0;
const int pinA1 = A1;
const int pinA2 = A2;
const int pinA3 = A3;
const int pinA4 = A4;
const int pinA5 = A5;

void setup() {
  // Set the pushbutton pins as input with internal pull-up resistors
  pinMode(pin2, INPUT_PULLUP);
  pinMode(pin3, INPUT_PULLUP);
  pinMode(pin4, INPUT_PULLUP);
  pinMode(pin5, INPUT_PULLUP);
  pinMode(pin6, INPUT_PULLUP);
  pinMode(pin7, INPUT_PULLUP);
  pinMode(pin8, INPUT_PULLUP);
  pinMode(pin9, INPUT_PULLUP);
  pinMode(pin10, INPUT_PULLUP);
  pinMode(pin11, INPUT_PULLUP);
  pinMode(pin12, INPUT_PULLUP);
  pinMode(pin13, INPUT_PULLUP);
  pinMode(pinA0, INPUT_PULLUP);
  pinMode(pinA1, INPUT_PULLUP);
  pinMode(pinA2, INPUT_PULLUP);
  pinMode(pinA3, INPUT_PULLUP);
  pinMode(pinA4, INPUT_PULLUP);
  pinMode(pinA5, INPUT_PULLUP);

  // Enable the PCINT interrupt group
  PCICR |= (1 << PCIE0);

  // Enable pin change interrupts for the desired pins
  PCMSK0 |= (1 << PCINT2) | (1 << PCINT3) | (1 << PCINT4) | (1 << PCINT5) | (1 << PCINT6) | (1 << PCINT7) |
           (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3) | (1 << PCINT4) | (1 << PCINT5);

  // Initialize the Keyboard library
  Keyboard.begin();
}

ISR(PCINT0_vect) {
  // Interrupt service routine for PCINT0 group
  if (digitalRead(pin2) == LOW) {
    Keyboard.press('a');
    Keyboard.release('a');
  } else if (digitalRead(pin3) == LOW) {
    Keyboard.press('b');
    Keyboard.release('b');
  } else if (digitalRead(pin4) == LOW) {
    Keyboard.press('c');
    Keyboard.release('c');
  } else if (digitalRead(pin5) == LOW) {
    Keyboard.press('d');
    Keyboard.release('d');
  } else if (digitalRead(pin6) == LOW) {
    Keyboard.press('e');
    Keyboard.release('e');
  } else if (digitalRead(pin7) == LOW) {
    Keyboard.press('f');
    Keyboard.release('f');
  } else if (digitalRead(pin8) == LOW) {
    Keyboard.press('g');
    Keyboard.release('g');
  } else if (digitalRead(pin9) == LOW) {
    Keyboard.press('h');
    Keyboard.release('h');
  } else if (digitalRead(pin10) == LOW) {
    Keyboard.press('i');
    Keyboard.release('i');
  } else if (digitalRead(pin11) == LOW) {
    Keyboard.press('j');
    Keyboard.release('j');
  } else if (digitalRead(pin12) == LOW) {
    Keyboard.press('k');
    Keyboard.release('k');
  } else if (digitalRead(pin13) == LOW) {
    Keyboard.press('l');
    Keyboard.release('l');
  } else if (digitalRead(pinA0) == LOW) {
    Keyboard.press('m');
    Keyboard.release('m');
  } else if (digitalRead(pinA1) == LOW) {
    Keyboard.press('n');
    Keyboard.release('n');
  } else if (digitalRead(pinA2) == LOW) {
    Keyboard.press('o');
    Keyboard.release('o');
  } else if (digitalRead(pinA3) == LOW) {
    Keyboard.press('p');
    Keyboard.release('p');
  } else if (digitalRead(pinA4) == LOW) {
    Keyboard.press('q');
    Keyboard.release('q');
  } else if (digitalRead(pinA5) == LOW) {
    Keyboard.press('r');
    Keyboard.release('r');
  }
}

void loop()
{
  // Nothing to do here, the ISR handles the button events
}

