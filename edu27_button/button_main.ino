/*
 * LED with button: interrupts and Bounce library.
 * the led on PIN 13 will change it's state 
 * after state of digital pin 2 has been changed
 */

#include <Bounce.h>

#define BUTTON 2
#define LED 13
#define TIME_CHATTER 50

int val=0;
int interruptPin = 0;           // Interrupt 0 is on DIGITAL PIN 2!
int main_flag = 0;

//create Bounce object. Set pin for button and time for bounce in milliseconds:
Bounce bouncer = Bounce(BUTTON, TIME_CHATTER); 

volatile int state = LOW;

void setup()                     
{
  pinMode(LED, OUTPUT);      // set led to OUTPUT
  pinMode(BUTTON, INPUT);       // set button to input  
  
  attachInterrupt(interruptPin, blink, FRONT); // link interrupt 0 to function blink().
  // attachInterrupt(interruptPin, stateChange, CHANGE); // link interrupt 0 to function blink().
  Serial.begin(9600);           // init serial
}

void loop()
{
  // do main loop here.
  // check_bounce();
  if (1 == main_flag)
  {
    digitalWrite(LED, state);          // show state  
    
    if (state)
      Serial.write("Button pressed. State = HIGH");
    else
      Serial.write("Button pressed. State = LOW");
    
    main_flag = 0;
  }
}

void check_bounce() 
{
  //if button change state:
  if ( bouncer.update() ) 
  {
    //if 1 read:
    if ( bouncer.read() == HIGH) 
    {      
     //reverce LED:
     if ( state == LOW ) 
     {
       state = HIGH;     
     }
     else 
     {
       state = LOW;
     } 
    }
  }
}



// ISR function
void stateChange()
{
  //noInterrupts();           // disable all interrupts
  state = !state;
  main_flag = 1;
  // Serial.write("press -- \n"); // 
  //digitalWrite(LED, state); 
  //interrupts();
}

// ISR function
void blink()
{
    static unsigned long millis_prev;
    if(millis() - 200 > millis_prev) 
    {
        state = !state;   // reverse
        main_flag = 1;
        millis_prev = millis();
    }
    
}

/*
void loop()                      
{
  val = digitalRead(BUTTON);    // got a button state
  if(val==HIGH)                 // it is pressed
  {
    digitalWrite(LED, HIGH); // led on
    //Serial.println("H");
  }
  else                          // button released
  {
    digitalWrite(LED, LOW);  // led OFF
    //Serial.println("L");
  }
  delay(100);
}

*/
