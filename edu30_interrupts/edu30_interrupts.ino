// Tinkercad + Arduino Lesson 8: External Interrupts
// https://www.youtube.com/watch?v=PIX6TZxukyo&list=PLKL6KBeCnI3X7cb1sznYuyScUesOxS-kL&index=9

ISR(INT0_vect)
{
   //code that runs when external interrupt 0 is triggered
   // check if pin 2 is HIGH (button not pressed, as pullup)
    if (PIND & 0b00000100) 
    {
      PORTD &= 0b01111111; // turn led off
    }
    else
    {
      PORTD |= 0b10000000; // turn led on
    }
}

int main(void)
{
  // setup code that only runs once
  DDRD = 0b10000000; // set pin 7 as output
  PORTD = 0b00000100; // enable internal pullup on pin 2 (UNO)

  // External Interrupt Control Register A
  EICRA = 0b00000001; // ant logical change in INT0

  // last 2 bits of a
  // External Interrupt Mask Register (set 1 to enable interrupt)
  EIMSK = 0b00000001; // enable INT0

  sei(); // enable interrupts globally

  // code that loops forever:
  while(1)
  {    
    /* 
    // check if pin 2 is HIGH (button not pressed, as pullup)
    if (PIND & 0b00000100) 
    {
      PORTD &= 0b01111111; // turn led off
    }
    else
    {
      PORTD |= 0b10000000; // turn led on
    }
    */ 

    // leave only long delay in main, to simulate long calculation:
    _delay_ms(5000);
  }
}

