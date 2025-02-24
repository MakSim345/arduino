#include <Keyboard.h>

// Define raw HID keycodes for numeric keypad digits

const int analogPinA0 = A0;
const int analogPinA1 = A1;
const int analogPinA2 = A2;
const int analogPinA3 = A3;

const int buttonPin1 = 1;
const int buttonPin2 = 2;
const int buttonPin3 = 3;
const int buttonPin4 = 4;
const int buttonPin5 = 5;
const int buttonPin6 = 6;
const int buttonPin7 = 7;
const int buttonPin8 = 8;
const int buttonPin9 = 9;
const int buttonPin14 = 14;
const int buttonPin15 = 15;

// Measured values for each button in A0
const int btn6ValueA0 = 860;
const int btn5ValueA0 = 829;
const int btn4ValueA0 = 780;
const int btn3ValueA0 = 701;
const int btn2ValueA0 = 541;
const int btn1ValueA0 = 65;
const int noPressValueA0 = 1023;

// Measured values for each button in A1
const int btn6ValueA1 = 860;
const int btn5ValueA1 = 830;
const int btn4ValueA1 = 780;
const int btn3ValueA1 = 720;
const int btn2ValueA1 = 539;
const int btn1ValueA1 = 60;
const int noPressValueA1 = 1023;

volatile bool button1Pressed = false;
volatile bool button1Released = false;

const int delayKeyPress = 40;
const int delayDebounce = 20;
const int resRange = 20; // range for resistors bridge

bool reverserON = false;
bool reverserOFF = true;

unsigned long button1PressTime = 0;
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
  pinMode(buttonPin1, INPUT_PULLUP); // TX - Pin 1
  pinMode(buttonPin2, INPUT_PULLUP); // Pin 2
  pinMode(buttonPin3, INPUT_PULLUP); // Pin 3
  pinMode(buttonPin4, INPUT_PULLUP); // Pin 4
  pinMode(buttonPin5, INPUT_PULLUP); // Pin 5
  pinMode(buttonPin6, INPUT_PULLUP); // Pin 6
  pinMode(buttonPin7, INPUT_PULLUP); // Pin 7
  pinMode(buttonPin8, INPUT_PULLUP); // Pin 8
  pinMode(buttonPin9, INPUT_PULLUP); // Pin 9
  pinMode(buttonPin14, INPUT_PULLUP); // Pin 14
  pinMode(buttonPin15, INPUT_PULLUP); // Pin 15

  // attachInterrupt(digitalPinToInterrupt(buttonPin1), button2ISR, FALLING);
  //attachInterrupt(digitalPinToInterrupt(buttonPin2), button1ISR, FALLING);

  Keyboard.begin();
  // Serial.begin(9600);
  // Serial.println("Arduino: Setup OK. Running...");
}

int readAnalogPortA0()
{
  int analogValue = analogRead(analogPinA0);  // Read the analog value from A0
  delay(delayDebounce);  // Debounce delay
  int retVal = 0;
  // Serial.println(analogValue);

   /* Checks the analog value against the measured values for each button.
    * If a match is found within a small range (+-resRange), it sends
    * the corresponding keystroke
    */
  if (analogValue >= btn1ValueA0 - resRange && analogValue <= btn1ValueA0 + resRange)
  {
    retVal = 1;
  }
  else if (analogValue >= btn2ValueA0 - resRange && analogValue <= btn2ValueA0 + resRange)
  {
    retVal = 2;
  }
  else if (analogValue >= btn3ValueA0 - resRange && analogValue <= btn3ValueA0 + resRange)
  {
    retVal = 3;
  }
  else if (analogValue >= btn4ValueA0 - resRange && analogValue <= btn4ValueA0 + resRange)
  {
    retVal = 4;
  }
  else if (analogValue >= btn5ValueA0 - resRange && analogValue <= btn5ValueA0 + resRange)
  {
    retVal = 5;
  }
  else if (analogValue >= btn6ValueA0 - resRange && analogValue <= btn6ValueA0 + resRange)
  {
    retVal = 6;
  }
  else if (analogValue >= noPressValueA0 - resRange && analogValue <= noPressValueA0 + resRange)
  {
    retVal = 0;
  }
  return retVal;
}


int readAnalogPortA1()
{
  int analogValue = analogRead(analogPinA1);  // Read the analog value from A0
  delay(delayDebounce);  // Debounce delay
  int retVal = 0;

  if (analogValue >= btn1ValueA1 - resRange && analogValue <= btn1ValueA1 + resRange)
  {
    retVal = 11;
  }
  else if (analogValue >= btn2ValueA1 - resRange && analogValue <= btn2ValueA1 + resRange)
  {
    retVal = 12;
  }
  else if (analogValue >= btn3ValueA1 - resRange && analogValue <= btn3ValueA1 + resRange)
  {
    retVal = 13;
  }
  else if (analogValue >= btn4ValueA1 - resRange && analogValue <= btn4ValueA1 + resRange)
  {
    retVal = 14;
  }
  else if (analogValue >= btn5ValueA1 - resRange && analogValue <= btn5ValueA1 + resRange)
  {
    retVal = 15;
  }
  else if (analogValue >= btn6ValueA1 - resRange && analogValue <= btn6ValueA1 + resRange)
  {
    retVal = 16;
  }
  else if (analogValue >= noPressValueA1 - resRange && analogValue <= noPressValueA1 + resRange)
  {
    retVal = 0;
  }

  return retVal;
}



void loop()
{
    int buttonNum = readAnalogPortA0();

    int buttonNumA1 = readAnalogPortA1();

    // Ind BRAKE: BACK
    if (1 == buttonNum)
    {
        // Keyboard.press('o');   // Simulate pressing "o"
        // delay(delayKeyPress); // Hold the key for a short duration
        // Keyboard.release('o'); // Release the key

        Keyboard.press('5');   // Simulate pressing "5"
        delay(delayKeyPress);  // Hold the key for a short duration
        Keyboard.release('5'); // Release the key
    }

    // Ind BRAKE: FORWARD
    if (2 == buttonNum)
    {
        // Keyboard.press(KEY_LEFT_SHIFT);
        // Keyboard.press('o');  // Simulate
        // delay(delayKeyPress); // Hold the key for a short duration
        // Keyboard.release(KEY_LEFT_SHIFT);
        // Keyboard.release('o'); // Release the key

        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press('5');  // Simulate "5"
        delay(delayKeyPress); // Hold the key for a short duration
        Keyboard.release(KEY_LEFT_SHIFT);
        Keyboard.release('5'); // Release the key
    }

    // Check Ind BRAKE BACK:
    if (3 == buttonNum)
    {
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press('6');  // Simulate "6"
        delay(delayKeyPress);           // Hold the key for a short duration
        Keyboard.release(KEY_LEFT_SHIFT);
        Keyboard.release('6'); // Release the key

        //Keyboard.press(KEY_LEFT_SHIFT);
        //Keyboard.press('i');  // Simulate
        //delay(delayKeyPress);           // Hold the key for a short duration
        //Keyboard.release(KEY_LEFT_SHIFT);
        //Keyboard.release('i'); // Release the key
    }

    // Check Ind BRAKE FORWARD:
    if (4 == buttonNum)
    {
        Keyboard.press('6');   // Simulate pressing "6"
        delay(delayKeyPress);  // Hold the key for a short duration
        Keyboard.release('6'); // Release the key

        //Keyboard.press('i');  // Simulate pressing "I"
        //delay(delayKeyPress); // Hold the key for a short duration
        //Keyboard.release('i'); // Release the key
    }

    // Check Throttle: FORWARD
    if (5 == buttonNum)
    {
        Keyboard.press('a');   // Simulate pressing "A"
        delay(delayKeyPress);             // Hold the key for a short duration
        Keyboard.release('a'); // Release the key
    }

    // Check Throttle: BACK
    if (6 == buttonNum)
    {
        Keyboard.press('d');   // Simulate pressing "D"
        delay(delayKeyPress);             // Hold the key for a short duration
        Keyboard.release('d'); // Release the key
    }

    // Check Pantograph: LEFT
    if (11 == buttonNumA1)
    {
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press('p');  // Simulate
        delay(delayKeyPress);           // Hold the key for a short duration
        Keyboard.release(KEY_LEFT_SHIFT);
        Keyboard.release('p'); // Release the key
    }

    // Check Pantograph: RIGHT
    if (12 == buttonNumA1)
    {
        Keyboard.press('p');   // Simulate pressing
        delay(delayKeyPress);             // Hold the key for a short duration
        Keyboard.release('p'); // Release the key
    }

    // Check Sand: LEFT
    if (13 == buttonNumA1)
    {
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press('x');  // Simulate
        delay(delayKeyPress);           // Hold the key for a short duration
        Keyboard.release(KEY_LEFT_SHIFT);
        Keyboard.release('x'); // Release the key
    }

    // Check Sand: RIGHT
    if (14 == buttonNumA1)
    {
        Keyboard.press('x');   // Simulate pressing
        delay(delayKeyPress);             // Hold the key for a short duration
        Keyboard.release('x'); // Release the key
    }

    // Check Wipes: LEFT
    if (15 == buttonNumA1)
    {
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press('v');  // Simulate
        delay(delayKeyPress); // Hold the key for a short duration
        Keyboard.release(KEY_LEFT_SHIFT);
        Keyboard.release('v'); // Release the key
    }
    // Check Wipes: RIGHT
    if (16 == buttonNumA1)
    {
        Keyboard.press('v');   // Simulate pressing
        delay(delayKeyPress); // Hold the key for a short duration
        Keyboard.release('v'); // Release the key
    }

    // Check if button AWS was pressed
    if (21 == buttonNumA1)
    {
        Keyboard.press('q');  // Simulate pressing "Q"
        delay(delayKeyPress); // Hold the key for a short duration
        Keyboard.release('q'); // Release the key
    }

    // Reverser: Key
    if (digitalRead(buttonPin2) == LOW)
    {
        if (!reverserON)
        {
            reverserON = true;
            reverserOFF = false;
            // Key turn right
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('W');  // Simulate press
            delay(delayKeyPress); // Hold the key for a short duration
            Keyboard.release(KEY_LEFT_CTRL);
            Keyboard.release('W'); // Release
        }
    }
    if (digitalRead(buttonPin2) == HIGH)
    {
        if (!reverserOFF)
        {
            reverserON = false;
            reverserOFF = true;
            // Key turn left
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('W');  // Simulate press
            delay(delayKeyPress); // Hold the key for a short duration
            Keyboard.release(KEY_LEFT_CTRL);
            Keyboard.release('W'); // Release
        }
    }

    // Warning Horn: BACK
    if (digitalRead(buttonPin14) == LOW)
    {
        Keyboard.press('n');   // Simulate pressing "N"
        delay(delayKeyPress);  // Hold the key for a short duration
        Keyboard.release('n'); // Release the key
    }

    // Warning Horn: FORWARD
    if (digitalRead(buttonPin15) == LOW)
    {
        Keyboard.press(' ');   // Simulate pressing "SPACE"
        delay(delayKeyPress);  // Hold the key for a short duration
        Keyboard.release(' '); // Release the key
    }

    // Check if Emergency Break button pressed
    // Note: #define KEY_BACKSPACE   0xB2
    if (digitalRead(buttonPin1) == LOW)
    {
        // Serial.println("Arduino: buttonPin1 == LOW");
        if (false == eKeyHeld)
        {
            eKeyHeld = true;
            BackspaceWasPressed = true;

            Keyboard.press(KEY_BACKSPACE);  // Simulate pressing BACKSPACE
            delay(delayKeyPress);  // Hold the key for a short duration
            Keyboard.release(KEY_BACKSPACE); // Release the key
            //Serial.println("Arduino: eKeyHeld == false");
        }
    }

    // Check if Emergency Break button released
    if (digitalRead(buttonPin1) == HIGH )
    {
        // Serial.println("Arduino: buttonPin1 == HIGH");
        if (true == BackspaceWasPressed)
        {
            eKeyHeld = false;
            BackspaceWasPressed = false;

            Keyboard.press(KEY_BACKSPACE);  // Simulate pressing BACKSPACE
            delay(delayKeyPress);  // Hold the key for a short duration
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

