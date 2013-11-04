/*
 * LED with button: interrupts and Bounce library.
 * the led on PIN 13 will change it's state 
 * after state of digital pin 2 has been changed
 */
// #include <Bounce.h>

#define BUTTON 2
#define LED 13
#define TIME_CHATTER 5

int val=0;
int interruptPin = 0;           // Interrupt 0 is on DIGITAL PIN 2!

volatile int state = LOW;

void setup()                     
{
  pinMode(LED, OUTPUT);      // set led to output
  // pinMode(BUTTON, INPUT);       // set button to input  
  
  //attachInterrupt(interruptPin, blink, CHANGE); // link interrupt 0 to function blink().
  attachInterrupt(interruptPin, stateChange, CHANGE); // link interrupt 0 to function blink().
  Serial.begin(9600);           // init serial
}

void loop()
{
  // do main loop here.
  digitalWrite(LED, state);          // show state  
}

// ISR function
void stateChange()
{
  //noInterrupts();           // disable all interrupts
  state = !state;
  // Serial.write("press -- \n"); // 
  //digitalWrite(LED, state); 
  //interrupts();
}

// ISR function
void blink()
{
    static unsigned long millis_prev;
    if(millis() - 100 > millis_prev) 
        state = !state;   // reverse
    
    millis_prev = millis();        
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
