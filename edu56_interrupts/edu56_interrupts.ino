#define ledPin 13      // internal LED pin
#define interruptPin 2 // A button between PIN-2 and GND

// a variable can change inside an ISR, thus must be volatile:
volatile byte state = LOW;
volatile int count = 0;
const int debounce_delay = 10; //100


void myISR_Blink()
{
    static unsigned long millis_prev;
    if (millis() - debounce_delay > millis_prev)
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
  // pinMode(interruptPin, INPUT_PULLUP); // pull-up to internal resistor and Vcc
  pinMode(interruptPin, INPUT); // use external resistor to pull-down (GND)

  // attachInterrupt(digitalPinToInterrupt(interruptPin), myISR_Blink, RISING);
  // attachInterrupt(0, myISR_Blink, CHANGE); // LED ON when button down and OFF on release
  attachInterrupt(0, myISR_Blink, RISING); // LED ON/OFF every time button pressed
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
