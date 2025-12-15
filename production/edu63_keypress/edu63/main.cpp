#include <Keyboard.h>
#include <Wire.h>

#define SLAVE_ADDRESS 0x08
#define BTN_RELEASE 255

const int analogPinA0 = A0;
const int analogPinA1 = A1;
const int analogPinA2 = A2;

const int buttonPin00 = 0;
const int buttonPin01 = 1;
const int buttonPin02 = 2;
const int buttonPin03 = 3;
const int buttonPin04 = 4;
const int buttonPin05 = 5;
const int buttonPin06 = 6;
const int buttonPin07 = 7;
const int buttonPin08 = 8;
const int buttonPin09 = 9;
const int buttonPin10 = 10;
const int buttonPin14 = 14;
const int buttonPin15 = 15;
const int buttonPin16 = 16;

// Measured values for each button in A0
const int btn6ValueA0 = 860;
const int btn5ValueA0 = 829;
const int btn4ValueA0 = 780;
const int btn3ValueA0 = 701;
const int btn2ValueA0 = 541;
const int btn1ValueA0 = 50;
const int noPressValueA0 = 1023;

// Measured values for each button in A1
const int btn6ValueA1 = 860;
const int btn5ValueA1 = 820;
const int btn4ValueA1 = 780;
const int btn3ValueA1 = 720;
const int btn2ValueA1 = 539;
const int btn1ValueA1 = 50;
const int noPressValueA1 = 1023;

// Measured values for each button in A2
const int btn6ValueA2 = 860;
const int btn5ValueA2 = 829;
const int btn4ValueA2 = 780;
const int btn3ValueA2 = 701;
const int btn2ValueA2 = 533;
const int btn1ValueA2 = 60;
const int noPressValueA2 = 1020;

const int delayDebounce = 20;
const int resRange = 20; // range for resistors bridge

bool reverserON = false;
bool reverserOFF = true;

bool CabLightsON = false;
bool CabLightsOFF = true;

bool engageWsKeyPressed = false;

bool headLightKeyFFPressed = false;
bool headLightKeyBKPressed = false;

bool hornKeyFFPressed = false;
bool hornKeyBKPressed = false;

bool throttleKeyFFPressed = false;
bool throttleKeyBBPressed = false;

bool autoBrakeKeyFFPressed = false;
bool autoBrakeKeyBBPressed = false;

bool indBrakeKeyFFPressed = false;
bool indBrakeKeyBBPressed = false;

bool engineOnKeyPressed = false;
bool engineOffKeyPressed = false;

bool awsKeyPressed = false;

bool pantographLeftKeyPressed = false;
bool pantographRigthKeyPressed = false;

bool sandLeftKeyPressed = false;
bool sandRigthKeyPressed = false;

bool wipersLeftKeyPressed = false;
bool wipersRigthKeyPressed = false;

const unsigned long longKeyPressDelay = 500;
const unsigned long keyPressDelay = 200;
const unsigned long shortPressDelay = 50;

//Note: this variable changes based on read of analogPinA1 state.
unsigned long reverserKnobHoldDuration = 500;

bool eKeyHeld = false; // Flag to track if 'e' key is being held
bool BackspaceWasPressed = false;

enum REVERSER_STATUS
{
    STATUS_O = 0,
    STATUS_F,
    STATUS_N,
    STATUS_R
};

REVERSER_STATUS Reverser_Current = STATUS_O;

bool Reverser_O_Set_Flag = false;
bool Reverser_F_Set_Flag = false;
bool Reverser_N_Set_Flag = false;
bool Reverser_R_Set_Flag = false;

int btnFromI2cMaster = BTN_RELEASE;

void setup()
{
  // Set pin modes
  pinMode(buttonPin00, INPUT_PULLUP); // Pin 0
  pinMode(buttonPin01, INPUT_PULLUP); // TX - Pin 1

  // Note: Pin2 and Pin3 are SDA and SCL for I2C
  // pinMode(buttonPin2, INPUT_PULLUP); // Pin 2
  // pinMode(buttonPin3, INPUT_PULLUP); // Pin 3

  pinMode(buttonPin04, INPUT_PULLUP); // Pin 4
  pinMode(buttonPin05, INPUT_PULLUP); // Pin 5
  pinMode(buttonPin06, INPUT_PULLUP); // Pin 6
  pinMode(buttonPin07, INPUT_PULLUP); // Pin 7
  pinMode(buttonPin08, INPUT_PULLUP); // Pin 8
  pinMode(buttonPin09, INPUT_PULLUP); // Pin 9
  pinMode(buttonPin10, INPUT_PULLUP); // Pin 10
  pinMode(buttonPin14, INPUT_PULLUP); // Pin 14
  pinMode(buttonPin15, INPUT_PULLUP); // Pin 15
  pinMode(buttonPin16, INPUT_PULLUP); // Pin 16

  pinMode(analogPinA1, INPUT_PULLUP); // Pin A1
  pinMode(analogPinA2, INPUT_PULLUP); // Pin A2

  // attachInterrupt(digitalPinToInterrupt(buttonPin1), button2ISR, FALLING);
  //attachInterrupt(digitalPinToInterrupt(buttonPin2), button1ISR, FALLING);

  Keyboard.begin();
  Wire.begin(SLAVE_ADDRESS); // Join the I2C bus as slave with address 0x08
  Wire.onReceive(receiveEvent_I2c); // Register the receive event

  // Serial.begin(9600);
  // Serial.println("Arduino: Setup OK. Running...");
  // Serial.println("Slave start to listen...");
}

void receiveEvent_I2c(int howMany)
{
  while (Wire.available())
  {
    int command = Wire.read(); // Receive the command from the master
    //Serial.println("Button pressed on master!");
    //Serial.println(command);
    btnFromI2cMaster = command;

    //if (command == 21)
    //{
      //Serial.println("Button 21 pressed on master!");
    //}
  }
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

int readAnalogPortA2()
{
  int analogValue = analogRead(analogPinA2);  // Read the analog value from A0
  delay(delayDebounce);  // Debounce delay
  int retVal = 0;

  if (analogValue >= btn1ValueA2 - resRange && analogValue <= btn1ValueA2 + resRange)
  {
    retVal = 21;
  }
  else if (analogValue >= btn2ValueA2 - resRange && analogValue <= btn2ValueA2 + resRange)
  {
    retVal = 22;
  }
  else if (analogValue >= btn3ValueA2 - resRange && analogValue <= btn3ValueA2 + resRange)
  {
    retVal = 23;
  }
  else if (analogValue >= btn4ValueA2 - resRange && analogValue <= btn4ValueA2 + resRange)
  {
    retVal = 24;
  }
  else if (analogValue >= btn5ValueA2 - resRange && analogValue <= btn5ValueA2 + resRange)
  {
    retVal = 25;
  }
  else if (analogValue >= btn6ValueA2 - resRange && analogValue <= btn6ValueA2 + resRange)
  {
    retVal = 26;
  }
  else if (analogValue >= noPressValueA2 - resRange && analogValue <= noPressValueA2 + resRange)
  {
    retVal = 0;
  }

  return retVal;
}

void loop()
{
    // Automatic BRAKE: BACK
    if (2 == btnFromI2cMaster)
    {
        if (false == autoBrakeKeyBBPressed)
        {
            autoBrakeKeyBBPressed = true;
            Keyboard.press('5');   // Simulate pressing "5"
        }
    }
    else if (2 != btnFromI2cMaster)
    {
        if (true == autoBrakeKeyBBPressed)
        {
            autoBrakeKeyBBPressed = false;
            Keyboard.release('5'); // Release the key
        }
    }

    // Automatic BRAKE: FORWARD
    if (1 == btnFromI2cMaster)
    {
        if (false == autoBrakeKeyFFPressed)
        {
            autoBrakeKeyFFPressed = true;
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press('5');  // Simulate "5"
        }
    }
    else if (1 != btnFromI2cMaster)
    {
        if (true == autoBrakeKeyFFPressed)
        {
            autoBrakeKeyFFPressed = false;
            Keyboard.release(KEY_LEFT_SHIFT);
            Keyboard.release('5'); // Release the key
        }
    }

    // Check Ind BRAKE BACK:
    if (4 == btnFromI2cMaster)
    {
        if (false == indBrakeKeyBBPressed)
        {
            indBrakeKeyBBPressed = true;
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press('6');
        }
    }
    else if (4 != btnFromI2cMaster)
    {
        if (true == indBrakeKeyBBPressed)
        {
            indBrakeKeyBBPressed = false;
            Keyboard.release(KEY_LEFT_SHIFT);
            Keyboard.release('6');
        }
    }

    // Check Ind BRAKE FORWARD:
    if (3 == btnFromI2cMaster)
    {
        if (false == indBrakeKeyFFPressed)
        {
            indBrakeKeyFFPressed = true;
            Keyboard.press('6');
        }
    }
    else if (3 != btnFromI2cMaster)
    {
        if (true == indBrakeKeyFFPressed)
        {
            indBrakeKeyFFPressed = false;
            Keyboard.release('6');
        }
    }

    // Throttle: FORWARD PRESS
    if (5 == btnFromI2cMaster)
    {
        if (false == throttleKeyFFPressed)
        {
            throttleKeyFFPressed = true;
            Keyboard.press('a');
        }
    }
    else if (5 != btnFromI2cMaster)
    {
        if (true == throttleKeyFFPressed)
        {
            throttleKeyFFPressed = false;
            Keyboard.release('a');
        }
    }

    // Throttle: BACK PRESS
    if (6 == btnFromI2cMaster)
    {
        if (false == throttleKeyBBPressed)
        {
            throttleKeyBBPressed = true;
            Keyboard.press('d');
        }
    }
    else if (6 != btnFromI2cMaster)
    {
        if (true == throttleKeyBBPressed)
        {
            throttleKeyBBPressed = false;
            Keyboard.release('d');
        }
    }

    // Check Pantograph: LEFT
    if (11 == btnFromI2cMaster)
    {
        if (false == pantographLeftKeyPressed)
        {
            pantographLeftKeyPressed = true;
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press('p');
        }
    }
    else if (11 != btnFromI2cMaster)
    {
        if (true == pantographLeftKeyPressed)
        {
            pantographLeftKeyPressed = false;
            Keyboard.release(KEY_LEFT_SHIFT);
            Keyboard.release('p');
        }
    }

    // Check Pantograph: RIGHT
    if (12 == btnFromI2cMaster)
    {
        if (false == pantographRigthKeyPressed)
        {
            pantographRigthKeyPressed = true;
            Keyboard.press('p');
        }
    }
    else if (12 != btnFromI2cMaster)
    {
        if (true == pantographRigthKeyPressed)
        {
            pantographRigthKeyPressed = false;
            Keyboard.release('p');
        }
    }

    // Check Sand: LEFT
    if (13 == btnFromI2cMaster)
    {
        if (false == sandLeftKeyPressed)
        {
            sandLeftKeyPressed = true;
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press('x');
        }
    }
    else if (13 != btnFromI2cMaster)
    {
        if (true == sandLeftKeyPressed)
        {
            sandLeftKeyPressed = false;
            Keyboard.release(KEY_LEFT_SHIFT);
            Keyboard.release('x');
        }
    }

    // Check Sand: RIGHT
    if (14 == btnFromI2cMaster)
    {
        if (false == sandRigthKeyPressed)
        {
            sandRigthKeyPressed = true;
            Keyboard.press('x');
        }
    }
    else if (14 != btnFromI2cMaster)
    {
        if (true == sandRigthKeyPressed)
        {
            sandRigthKeyPressed = false;
            Keyboard.release('x');
        }
    }

    // Check Wipes: LEFT
    if (15 == btnFromI2cMaster)
    {
        if (false == wipersLeftKeyPressed)
        {
            wipersLeftKeyPressed = true;
            //Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press('V');
            delay(shortPressDelay);
        }
    }
    else if (15 != btnFromI2cMaster)
    {
        if (true == wipersLeftKeyPressed)
        {
            wipersLeftKeyPressed = false;
            //Keyboard.release(KEY_LEFT_SHIFT);
            Keyboard.release('V');
        }
    }
    // Check Wipes: RIGHT
    if (16 == btnFromI2cMaster)
    {
        if (false == wipersRigthKeyPressed)
        {
            wipersRigthKeyPressed = true;
            Keyboard.press('v');
            delay(shortPressDelay);
        }
    }
    else if (16 != btnFromI2cMaster)
    {
        if (true == wipersRigthKeyPressed)
        {
            wipersRigthKeyPressed = false;
            Keyboard.release('v');
        }
    }

    // Check if button AWS was pressed
    if (21 == btnFromI2cMaster)
    {
        if (false == awsKeyPressed)
        {
            awsKeyPressed = true;
            Keyboard.press('q');
        }
    }
    else if (21 != btnFromI2cMaster)
    {
        if (true == awsKeyPressed)
        {
            awsKeyPressed = false;
            Keyboard.release('q');
        }
    }

    // Check if button Engine_ON was pressed
    if (22 == btnFromI2cMaster)
    {
        if (false == engineOnKeyPressed)
        {
            engineOnKeyPressed = true;
            Keyboard.press('z');
        }
    }
    else if (22 != btnFromI2cMaster)
    {
        if (true == engineOnKeyPressed)
        {
            engineOnKeyPressed = false;
            Keyboard.release('z'); // Release the key
        }
    }

    // Check if button Engine_OFF was pressed
    if (23 == btnFromI2cMaster)
    {
        if (false == engineOffKeyPressed)
        {
            engineOffKeyPressed = true;
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press('z');
            delay(shortPressDelay);
        }
    }
    else if (23 != btnFromI2cMaster)
    {
        if (true == engineOffKeyPressed)
        {
            engineOffKeyPressed = false;
            Keyboard.release(KEY_LEFT_SHIFT);
            Keyboard.release('z'); // Release the key
        }
      }

    // Check if button Left Doors was pressed
    if (24 == btnFromI2cMaster)
    {
        Keyboard.press('y');
        delay(keyPressDelay);
        Keyboard.release('y');
    }

    // Check if button All Doors was pressed
    if (26 == btnFromI2cMaster)
    {
        Keyboard.press(KEY_TAB);
        delay(shortPressDelay);
        Keyboard.press(KEY_KP_ENTER);

        delay(keyPressDelay);
        
        Keyboard.release(KEY_TAB);
        delay(shortPressDelay);
        Keyboard.release(KEY_KP_ENTER);
    }

    // Check if button RIGHT Doors was pressed
    if (25 == btnFromI2cMaster)
    {
        Keyboard.press('u');
        delay(keyPressDelay);
        Keyboard.release('u');
    }

/* Digital Pins Read */
/*************************************************************************/
    // Set the Reverser knob delay: long or short.
    // It is based on position of a button connected to pin A1.
    if (digitalRead(analogPinA1) == LOW)
    {
       reverserKnobHoldDuration = longKeyPressDelay;
    }
    else  //else if (digitalRead(analogPinA1) == HIGH)
    {
       reverserKnobHoldDuration = keyPressDelay;
    }

    // Reverser: Main Key
    if (digitalRead(buttonPin00) == LOW)
    {
        if (!reverserON)
        {
            reverserON = true;
            reverserOFF = false;
            // Key turn right
            Keyboard.press(KEY_KP_MINUS);
            delay(keyPressDelay);
             Keyboard.release(KEY_KP_MINUS);
            /*
            Keyboard.press(KEY_LEFT_CTRL);
            delay(keyPressDelay);
            //delay(longKeyPressDelay);
            Keyboard.press('w');
            delay(2*longKeyPressDelay);
            //delay(keyPressDelay);
            Keyboard.release('w');
            Keyboard.release(KEY_LEFT_CTRL);
            */
        }
    }
    if (digitalRead(buttonPin00) == HIGH)
    {
        if (!reverserOFF)
        {
            reverserON = false;
            reverserOFF = true;
            // Key turn left
            Keyboard.press(KEY_KP_MINUS);
            delay(keyPressDelay);
            Keyboard.release(KEY_KP_MINUS);
            /*
            Keyboard.press(KEY_LEFT_CTRL);
            //delay(longKeyPressDelay);
            delay(keyPressDelay);
            Keyboard.press('w');
            delay(2*longKeyPressDelay);
            //delay(keyPressDelay);
            Keyboard.release('w');
            Keyboard.release(KEY_LEFT_CTRL);
            */
        }
    }

//------------------------------------------------------
    // Reverser O:
    if (digitalRead(buttonPin07) == LOW)
    {
        if (false == Reverser_O_Set_Flag)
        {
            Keyboard.press('w');
            delay(reverserKnobHoldDuration);
            Keyboard.release('w');

            Reverser_Current = STATUS_O;
            Reverser_O_Set_Flag = true;
            Reverser_F_Set_Flag = false;
            Reverser_N_Set_Flag = false;
            Reverser_R_Set_Flag = false;
        }
    }
    // Reverser F:
    if (digitalRead(buttonPin04) == LOW)
    {
        if (false == Reverser_F_Set_Flag)
        {
            if (STATUS_O == Reverser_Current)
            {
                Keyboard.press('s');
                delay(reverserKnobHoldDuration);
                Keyboard.release('s');
            }
            if (STATUS_R == Reverser_Current)
            {
                Keyboard.press('w');
                delay(reverserKnobHoldDuration);
                Keyboard.release('w');

                delay(keyPressDelay);

                Keyboard.press('w');
                delay(reverserKnobHoldDuration);
                Keyboard.release('w');
            }
            if (STATUS_N == Reverser_Current)
            {
                Keyboard.press('w');
                delay(reverserKnobHoldDuration);
                Keyboard.release('w');
            }
            Reverser_Current = STATUS_F;
            Reverser_O_Set_Flag = false;
            Reverser_F_Set_Flag = true;
            Reverser_N_Set_Flag = false;
            Reverser_R_Set_Flag = false;
        }
    }
    // Reverser N:
    if (digitalRead(buttonPin05) == LOW)
    {
        if (false == Reverser_N_Set_Flag)
        {
            if (STATUS_F == Reverser_Current)
            {
                Keyboard.press('s');
                delay(reverserKnobHoldDuration);
                Keyboard.release('s');
            }
            if (STATUS_O == Reverser_Current)
            {
                Keyboard.press('s');
                delay(reverserKnobHoldDuration);
                Keyboard.release('s');

                delay(keyPressDelay);

                Keyboard.press('s');
                delay(reverserKnobHoldDuration);
                Keyboard.release('s');
            }
            if (STATUS_R == Reverser_Current)
            {
                Keyboard.press('w');
                delay(reverserKnobHoldDuration);
                Keyboard.release('w');
            }

            Reverser_Current = STATUS_N;
            Reverser_O_Set_Flag = false;
            Reverser_F_Set_Flag = false;
            Reverser_N_Set_Flag = true;
            Reverser_R_Set_Flag = false;
        }
    }
    // Reverser R:
    if (digitalRead(buttonPin06) == LOW)
    {
        if (false == Reverser_R_Set_Flag)
        {
            Keyboard.press('s');
            delay(reverserKnobHoldDuration);
            Keyboard.release('s');

            Reverser_Current = STATUS_R;
            Reverser_O_Set_Flag = false;
            Reverser_F_Set_Flag = false;
            Reverser_N_Set_Flag = false;
            Reverser_R_Set_Flag = true;
        }
    }
//------------------------------------------------------

    // Cabin LIGHTS: LEFT
    if (digitalRead(buttonPin08) == LOW)
    {
        if (!CabLightsON)
        {
            CabLightsON = true;
            CabLightsOFF = false;
            Keyboard.press('l');
            delay(keyPressDelay);
            Keyboard.release('l');
        }
    }
    // Cabin LIGHTS: RIGHT
    if (digitalRead(buttonPin08) == HIGH)
    {
        if (!CabLightsOFF)
        {
            CabLightsON = false;
            CabLightsOFF = true;
            Keyboard.press('l');
            delay(keyPressDelay);
            Keyboard.release('l');
        }
    }

    /* Head Lights. Uses 2 pins, i.e. 10 and 16.
     * For every pin we need to check high and low signals
     * Total 4 if-cases.
     */
    // Head Lights: LEFT
    if (digitalRead(buttonPin16) == LOW)
    {
        if (false == headLightKeyFFPressed)
        {
            headLightKeyFFPressed = true;
            // Simulate pressing "Shift+h"
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press('h');
        }
    }
    if (digitalRead(buttonPin16) == HIGH)
    {
        if (true == headLightKeyFFPressed)
        {
            headLightKeyFFPressed = false;
            // Release keys
            Keyboard.release(KEY_LEFT_SHIFT);
            Keyboard.release('h');
        }
    }
    // Head Lights: RIGHT
    if (digitalRead(buttonPin10) == LOW)
    {
        if (false == headLightKeyBKPressed)
        {
            headLightKeyBKPressed = true;
            // Simulate pressing "h"
            Keyboard.press('h');
        }
    }
    if (digitalRead(buttonPin10) == HIGH)
    {
        if (true == headLightKeyBKPressed)
        {
            headLightKeyBKPressed = false;
            // Release the key
            Keyboard.release('h');
        }
    }

    /* Engage Warning System: Use only one pin, i.e. Pin9
     * The button has 3 state, but two states are connected together
     */
    // Engage Warning System: PRESS
    if (digitalRead(buttonPin09) == LOW)
    {

        if (false == engageWsKeyPressed)
        {
            engageWsKeyPressed = true;
            //Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_KP_PLUS);
            // Note: key counts as pressed until the button released!
            //delay(keyPressDelay);
        }
    }
    // Engage Warning System: RELEASE
    if (digitalRead(buttonPin09) == HIGH)
    {
        if (true == engageWsKeyPressed)
        {
            engageWsKeyPressed = false;
            Keyboard.release(KEY_KP_PLUS);
        }
    }

    /* Warning HORN. Uses 2 pins, i.e. 14 and 15.
     * For every pin we need to check high and low signals
     * Total 4 if-cases.
     */
    // Warning Horn: FORWARD PRESS
    if (digitalRead(buttonPin15) == LOW)
    {
        if (false == hornKeyFFPressed)
        {
            hornKeyFFPressed = true;
            // Simulate pressing "N":
            Keyboard.press('n');
        }
    }
    // Warning Horn: FORWARD RELEASE
    if (digitalRead(buttonPin15) == HIGH)
    {
        if (true == hornKeyFFPressed)
        {
            hornKeyFFPressed = false;
            // Release the key
            Keyboard.release('n');
        }
    }

    // Warning Horn: BACK PRESS
    if (digitalRead(buttonPin14) == LOW)
    {
        if (false == hornKeyBKPressed)
        {
            hornKeyBKPressed = true;
            Keyboard.press(' ');   // Simulate pressing "SPACE"
         }
    }

    // Warning Horn: BACK RELEASE
    if (digitalRead(buttonPin14) == HIGH)
    {
        if (true == hornKeyBKPressed)
        {
            hornKeyBKPressed = false;
            Keyboard.release(' '); // Release the key
        }
    }

    // Check if Emergency Break button pressed
    // Note: #define KEY_BACKSPACE   0xB2
    if (digitalRead(buttonPin01) == LOW)
    {
        // Serial.println("Arduino: buttonPin1 == LOW");
        if (false == eKeyHeld)
        {
            eKeyHeld = true;
            BackspaceWasPressed = true;

            Keyboard.press(KEY_BACKSPACE);  // Simulate pressing BACKSPACE
            delay(keyPressDelay);  // Hold the key for a short duration
            Keyboard.release(KEY_BACKSPACE); // Release the key
            //Serial.println("Arduino: eKeyHeld == false");
        }
    }

    // Check if Emergency Break button released
    if (digitalRead(buttonPin01) == HIGH )
    {
        // Serial.println("Arduino: buttonPin1 == HIGH");
        if (true == BackspaceWasPressed)
        {
            eKeyHeld = false;
            BackspaceWasPressed = false;

            Keyboard.press(KEY_BACKSPACE);  // Simulate pressing BACKSPACE
            delay(keyPressDelay);  // Hold the key for a short duration
            Keyboard.release(KEY_BACKSPACE); // Release the key

            // Serial.println("Arduino: BackspaceWasPressed == true");
        }
    }
}

/*
#include <Keyboard.h>

const int buttonPin = 2;  // Pin where the button is connected
int buttonState = 0;       // Variable to hold the button state

volatile bool button1Pressed = false;
volatile bool button1Released = false;

unsigned long lastDebounceTime1 = 0; // Last debounce time for button 1
const unsigned long debounceDelay = 50; // Debounce delay in milliseconds

unsigned long button1PressTime = 0;
bool waitForRelease = false; // Flag for ignore fixed button release

void setup()
{
  pinMode(buttonPin, INPUT_PULLUP);  // Set button pin as input with internal pull-up
  Keyboard.begin();                   // Start the Keyboard library
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

