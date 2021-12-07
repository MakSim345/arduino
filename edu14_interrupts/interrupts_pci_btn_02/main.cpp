/*
Tinkercad + Arduino Lesson 9: Pin Change Interrupts
"https://www.youtube.com/watch?v=7aAJMvSNpEA"

NOTE: does not compile for Leonardo!
Error: "'PCMSK1' was not declared in this scope"

Note for PCIE2:
Any change on any enabled PCINT[23:16] pin will cause an interrupt
This is unlike attachInterrupt() function, 
where we can choose RISING, FALL etc.
*/

#define GREENLED 12
#define BLUELED  10
#define REDLED   8
#define WHITELED 6

#define BUTTON_04 4
#define BUTTON_13 13

#ifdef MAIN_IN_USE_YS_2021

volatile int state = LOW;

/*
 * ISR 
*/
ISR(PCINT2_vect)
{ 
  // code that runs when pin change interrupt 2 is triggered
  PORTD ^= 0b10000000; // toggle pin 7
}


int main()
{
  // setup code that only runs once:
  DDRD =  0b10000000; // set pin 7 as output (Register D, PD7, PCINT23)
  PORTD = 0b11100000; // enable pullup on pins 5,6,7 - Port D

  PCICR  |= 0b00000100; // enable PORT D for PCI
  PCMSK2 |= 0b01100000; // enable INT22 and INT21

  sei(); // enable interrupts globally

  while(1)
  {
      // code that loops forever
      _delay_ms (5000);
  }
}
#endif

