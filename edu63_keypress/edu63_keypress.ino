#include <Keyboard.h>

const int buttonPin1 = 1; // Button connected to pin 1
const int buttonPin2 = 2; // Button connected to pin 2

const int buttonPin8 = 8;
const int buttonPin9 = 9;

volatile bool button1Pressed = false;
volatile bool button2Pressed = false;
volatile bool button1Released = false;

unsigned long button1PressTime = 0;
// const unsigned long holdDuration = 3000; // 3 seconds
const unsigned long holdDuration = 2000; // 2 seconds
bool eKeyHeld = false; // Flag to track if 'e' key is being held
bool BackspaceWasPressed = false;
bool waitForRelease = false; // Flag for ignore fixed button release

// Debounce variables
unsigned long lastDebounceTime1 = 0; // Last debounce time for button 1
unsigned long lastDebounceTime2 = 0; // Last debounce time for button 2
const unsigned long debounceDelay = 50; // Debounce delay in milliseconds

void setup()
{
  // Set pin modes
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin8, INPUT_PULLUP);  // Use internal pull-up resistor
  pinMode(buttonPin9, INPUT_PULLUP);  // Use internal pull-up resistor

  // attachInterrupt(digitalPinToInterrupt(buttonPin1), button2ISR, FALLING);
  //attachInterrupt(digitalPinToInterrupt(buttonPin2), button1ISR, FALLING);

  Keyboard.begin();
  // Serial.begin(9600);
  // Serial.println("Arduino: Setup OK. Running...");
}

void loop()
{
    // Check if pin 8 is pressed
    if (digitalRead(buttonPin8) == LOW)
    {
        Keyboard.press('n');  // Simulate pressing "N"
        delay(50);           // Hold the key for a short duration
        Keyboard.release('n'); // Release the key
    }

    // Check if pin 9 is pressed
    if (digitalRead(buttonPin9) == LOW)
    {
        Keyboard.press(' ');  // Simulate pressing "SPACE"
        delay(50);                 // Hold the key for a short duration
        Keyboard.release(' '); // Release the key
    }

    // Check if button 2 was pressed
    if (digitalRead(buttonPin2) == LOW)
    {
        Keyboard.press('q');  // Simulate pressing "Q"
        delay(50);           // Hold the key for a short duration
        Keyboard.release('q'); // Release the key
    }

    // Check if button 1 was pressed
    // Note: #define KEY_BACKSPACE   0xB2
    if (digitalRead(buttonPin1) == LOW)
    {
        // Serial.println("Arduino: buttonPin1 == LOW");
        if (false == eKeyHeld)
        {
            eKeyHeld = true;
            BackspaceWasPressed = true;

            Keyboard.press(KEY_BACKSPACE);  // Simulate pressing BACKSPACE
            delay(50);           // Hold the key for a short duration
            Keyboard.release(KEY_BACKSPACE); // Release the key
            //Serial.println("Arduino: eKeyHeld == false");
        }
    }

    // Check if button 1 was released
    if (digitalRead(buttonPin1) == HIGH )
    {
        // Serial.println("Arduino: buttonPin1 == HIGH");
        if (true == BackspaceWasPressed)
        {
            eKeyHeld = false;
            BackspaceWasPressed = false;

            Keyboard.press(KEY_BACKSPACE);  // Simulate pressing BACKSPACE
            delay(50);           // Hold the key for a short duration
            Keyboard.release(KEY_BACKSPACE); // Release the key

            // Serial.println("Arduino: BackspaceWasPressed == true");
        }

    }
}

// Interrupt service routine for button 1 (e)
void button1ISR()
{
  unsigned long currentTime = millis();
  // Check if debounce delay has passed
  if ((currentTime - lastDebounceTime1) > debounceDelay)
  {
    lastDebounceTime1 = currentTime; // Update the last debounce time

    if (waitForRelease)
    {
        button1Released = true;
        waitForRelease = false;
    }
    else if (!eKeyHeld) // Ignore if 'e' key is already being held
    {
      button1Pressed = true;
      button1PressTime = millis(); // Record the time when button 1 was pressed
    }
  }
}

// Interrupt service routine for button 2 (q)
void button2ISR()
{
  unsigned long currentTime = millis();
  // Check if debounce delay has passed
  if ((currentTime - lastDebounceTime2) > debounceDelay)
  {
    button2Pressed = true;
    lastDebounceTime2 = currentTime; // Update the last debounce time
  }
}




/*

#include <Keyboard.h>

const int buttonPin = 2;  // Pin where the button is connected
int buttonState = 0;       // Variable to hold the button state

void setup()
{
  pinMode(buttonPin, INPUT_PULLUP);  // Set button pin as input with internal pull-up
  Keyboard.begin();                   // Start the Keyboard library
}

void loop()
{
  buttonState = digitalRead(buttonPin); // Read the state of the button

  // Check if the button is pressed
  if (buttonState == LOW)
  { // Button is pressed (active low)
    Keyboard.press('e');    // Simulate pressing the 'E' key
    delay(2000);             // Wait for a short period to ensure the key press is registered
    Keyboard.release('e');  // Release the 'E' key
    delay(500);             // Debounce delay
  }
}

*/

