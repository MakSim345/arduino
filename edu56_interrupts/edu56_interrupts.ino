#define ledPin 13      // internal LED pin 
#define interruptPin 2 // A button between PIN-2 and GND

// a variable can change inside an ISR, thus must be volatile:
volatile byte state = LOW;
volatile int count = 0;  

void myISR_Blink() 
{
    static unsigned long millis_prev;
    if (millis() - 100 > millis_prev) 
    {
        state = !state;   // reverse
    }
    
    millis_prev = millis();        
}

void setup() 
{
  Serial.begin(9600);
  Serial.println("Started");
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP); // pull up to internal resistor and Vcc
  // pinMode(interruptPin, INPUT);
  // attachInterrupt(digitalPinToInterrupt(interruptPin), myISR_Blink, RISING);
  attachInterrupt(0, myISR_Blink, CHANGE);
}

void loop() 
{
  if (state)
  {
    Serial.println("STATE = HIGH");
  }
  else
  {
    Serial.println("STATE = LOW");
  }
  digitalWrite(ledPin, state);
  // delay(500);
  // digitalWrite(ledPin, HIGH);
}
