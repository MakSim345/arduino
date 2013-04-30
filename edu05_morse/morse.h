#ifndef Morse_h
#define Morse_h
/*
  morse.h - Library for flashing Morse code.
*/

#include "Arduino.h"

class Morse
{
  public:
    Morse(int pin);
    void dot();
    void dash();

  private:
    int _pin;
};

#endif
