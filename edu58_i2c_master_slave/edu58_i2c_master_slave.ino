#include <Wire.h>

#define SLAVE_ADDRESS 0x08
#define BUTTON_PIN 7
#define BTN_RELEASE 255

/***Define ONLY ONE of it:**/
#define MASTER
// #define SLAVE
/***************************/

//Disable if buttonPin01 is in use:
#define DEBUG_PRINT


#ifdef MASTER

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
const int buttonPin11 = 11;
const int buttonPin12 = 12;

// const int buttonPin13 = 13; // Do not use! It is internal LED in NANO

const int buttonPin14 = 14;
const int buttonPin15 = 15;
const int buttonPin16 = 16;
const int buttonPin17 = 17;

// Pins 18 and 19
// On NANO - Follow are used for I2C only!
// On MEGA - use as normal GPIO
const int buttonPin18 = 18;
const int buttonPin19 = 19;

// Note: No internal PULLUP here! Ise a real resistor.
const int buttonPin20 = A6;
const int buttonPin21 = A7;

const int buttonPin22 = 22;
const int buttonPin24 = 24;
const int buttonPin26 = 26;
const int buttonPin28 = 28;

const int buttonPin32 = 32;
const int buttonPin34 = 34;

bool buttonPin01ON = false;
bool buttonPin01OFF = true;

bool buttonPin02ON = false;
bool buttonPin02OFF = true;

bool buttonPin03ON = false;
bool buttonPin03OFF = true;

bool buttonPin04ON = false;
bool buttonPin04OFF = true;

bool buttonPin05ON = false;
bool buttonPin05OFF = true;

bool buttonPin06ON = false;
bool buttonPin06OFF = true;

bool buttonPin07ON = false;
bool buttonPin07OFF = true;

bool buttonPin08ON = false;
bool buttonPin08OFF = true;

bool buttonPin09ON = false;
bool buttonPin09OFF = true;

bool buttonPin10ON = false;
bool buttonPin10OFF = true;

bool buttonPin11ON = false;
bool buttonPin11OFF = true;

bool buttonPin12ON = false;
bool buttonPin12OFF = true;

bool buttonPin14ON = false;
bool buttonPin14OFF = true;

bool buttonPin15ON = false;
bool buttonPin15OFF = true;

bool buttonPin16ON = false;
bool buttonPin16OFF = true;

bool buttonPin17ON = false;
bool buttonPin17OFF = true;

bool buttonPin18ON = false;
bool buttonPin18OFF = true;

bool buttonPin19ON = false;
bool buttonPin19OFF = true;

bool buttonPin20ON = false;
bool buttonPin20OFF = true;

bool buttonPin21ON = false;
bool buttonPin21OFF = true;

bool buttonPin22ON = false;
bool buttonPin22OFF = true;

bool buttonPin24ON = false;
bool buttonPin24OFF = true;

bool buttonPin26ON = false;
bool buttonPin26OFF = true;

bool buttonPin28ON = false;
bool buttonPin28OFF = true;

bool buttonPin32ON = false;
bool buttonPin32OFF = true;

bool buttonPin34ON = false;
bool buttonPin34OFF = true;

unsigned long button1PressTime = 0;
const unsigned long holdDuration = 2000; // 2 seconds
const unsigned long keyPressDelay = 200; //ms
const int delayKeyPress = 250;
const int delayKeyPress50 = 50;


void setup()
{
  //pinMode(BUTTON_PIN, INPUT_PULLUP); // Set the button pin as input with internal pull-up
  // Set pin modes
  pinMode(buttonPin01, INPUT_PULLUP); // TX - Pin 1
  pinMode(buttonPin02, INPUT_PULLUP); // Pin 2
  pinMode(buttonPin03, INPUT_PULLUP); // Pin 3
  pinMode(buttonPin04, INPUT_PULLUP); // Pin 4
  pinMode(buttonPin05, INPUT_PULLUP); // Pin 5
  pinMode(buttonPin06, INPUT_PULLUP); // Pin 6

  pinMode(buttonPin07, INPUT_PULLUP); // Pin 7
  pinMode(buttonPin08, INPUT_PULLUP); // Pin 8
  pinMode(buttonPin09, INPUT_PULLUP); // Pin 9
  pinMode(buttonPin10, INPUT_PULLUP); // Pin 10
  pinMode(buttonPin11, INPUT_PULLUP); // Pin 11
  pinMode(buttonPin12, INPUT_PULLUP); // Pin 12

  pinMode(buttonPin14, INPUT_PULLUP); // Pin 14
  pinMode(buttonPin15, INPUT_PULLUP); // Pin 15
  pinMode(buttonPin16, INPUT_PULLUP); // Pin 16
  pinMode(buttonPin17, INPUT_PULLUP); // Pin 17

  // Follow are I2C Pins in NANO, but in MEGA it is ok:
  pinMode(buttonPin18, INPUT_PULLUP); // Pin 18
  pinMode(buttonPin19, INPUT_PULLUP); // Pin 19

  // No PULLUP for A6 and A7. Use real resistor
  // pinMode(buttonPin20, INPUT_PULLUP); // Pin 20
  // pinMode(buttonPin21, INPUT_PULLUP); // Pin 21

  pinMode(buttonPin22, INPUT_PULLUP); // Pin 22
  pinMode(buttonPin24, INPUT_PULLUP); // Pin 24
  pinMode(buttonPin26, INPUT_PULLUP); // Pin 26
  pinMode(buttonPin28, INPUT_PULLUP); // Pin 28

  pinMode(buttonPin32, INPUT_PULLUP); // Pin 32
  pinMode(buttonPin34, INPUT_PULLUP); // Pin 34

  Wire.begin(); // Join the I2C bus as master
#ifdef DEBUG_PRINT
  Serial.begin(9600); // Start the serial monitor
  Serial.println("Master ready to send...");
#endif //DEBUG_PRINT
}

void loop()
{
   if (digitalRead(buttonPin01) == LOW)
    {
        // Note: Pin1 is Serial TX, no output here!
        if (!buttonPin01ON)
        {
            buttonPin01ON = true;
            buttonPin01OFF = false;
            Serial.println("Master sending Pantograph_L Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(11); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin01) == HIGH)
    {
        if (!buttonPin01OFF)
        {
            buttonPin01ON = false;
            buttonPin01OFF = true;
            Serial.println("Master sending Pantograph_L Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    if (digitalRead(buttonPin02) == LOW)
    {
        if (!buttonPin02ON)
        {
            buttonPin02ON = true;
            buttonPin02OFF = false;
            Serial.println("Master sending Pantograph_R Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(12); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    if (digitalRead(buttonPin02) == HIGH)
    {
        if (!buttonPin02OFF)
        {
            buttonPin02ON = false;
            buttonPin02OFF = true;
            Serial.println("Master sending Pantograph_R Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    // Pin03 - Sand_L
    if (digitalRead(buttonPin03) == LOW)
    {
        if (!buttonPin03ON)
        {
            buttonPin03ON = true;
            buttonPin03OFF = false;
            Serial.println("Master sending Sand_L Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(13); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    if (digitalRead(buttonPin03) == HIGH)
    {
        if (!buttonPin03OFF)
        {
            buttonPin03ON = false;
            buttonPin03OFF = true;
            Serial.println("Master sending Sand_L Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    // Pin04 - Sand_R
    if (digitalRead(buttonPin04) == LOW)
    {
        if (!buttonPin04ON)
        {
            buttonPin04ON = true;
            buttonPin04OFF = false;
            Serial.println("Master sending Sand_R Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(14); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    if (digitalRead(buttonPin04) == HIGH)
    {
        if (!buttonPin04OFF)
        {
            buttonPin04ON = false;
            buttonPin04OFF = true;
            Serial.println("Master sending Sand_R Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    // Pin05 -
    if (digitalRead(buttonPin05) == LOW)
    {
        if (!buttonPin05ON)
        {
            buttonPin05ON = true;
            buttonPin05OFF = false;
            Serial.println("Master sending Wipes_L Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(15); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    if (digitalRead(buttonPin05) == HIGH)
    {
        if (!buttonPin05OFF)
        {
            buttonPin05ON = false;
            buttonPin05OFF = true;
            Serial.println("Master sending Wipes_L Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    // Pin06 -
    if (digitalRead(buttonPin06) == LOW)
    {
        if (!buttonPin06ON)
        {
            buttonPin06ON = true;
            buttonPin06OFF = false;
            Serial.println("Master sending Wipes_R Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(16); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    if (digitalRead(buttonPin06) == HIGH)
    {
        if (!buttonPin06OFF)
        {
            buttonPin06ON = false;
            buttonPin06OFF = true;
            Serial.println("Master sending Wipes R Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }


   /*************************************************************/
   /****Low Joysticks - 3 items **/
    // Pin07 - Auto_Brake FF
    if (digitalRead(buttonPin07) == LOW)
    {
        if (!buttonPin07ON)
        {
            buttonPin07ON = true;
            buttonPin07OFF = false;
            Serial.println("Master sending Auto_Brake FF Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(2); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin07) == HIGH)
    {
        if (!buttonPin07OFF)
        {
            buttonPin07ON = false;
            buttonPin07OFF = true;
            Serial.println("Master sending Auto_Brake FF Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    // Pin08 - Auto_Brake Back
    if (digitalRead(buttonPin08) == LOW)
    {
        if (!buttonPin08ON)
        {
            buttonPin08ON = true;
            buttonPin08OFF = false;
            Serial.println("Master sending Auto_Brake Back Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(1); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin08) == HIGH)
    {
        if (!buttonPin08OFF)
        {
            buttonPin08ON = false;
            buttonPin08OFF = true;
            Serial.println("Master sending Auto_Brake Back Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    // Pin09 - Ind BRAKE FORWARD
    if (digitalRead(buttonPin09) == LOW)
    {
        if (!buttonPin09ON)
        {
            buttonPin09ON = true;
            buttonPin09OFF = false;
            Serial.println("Master sending Ind BRAKE FF Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(3); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin09) == HIGH)
    {
        if (!buttonPin09OFF)
        {
            buttonPin09ON = false;
            buttonPin09OFF = true;
            Serial.println("Master sending Ind BRAKE FF Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    // Pin10 - Ind BRAKE BACK:
    if (digitalRead(buttonPin10) == LOW)
    {
        if (!buttonPin10ON)
        {
            buttonPin10ON = true;
            buttonPin10OFF = false;
            Serial.println("Master sending Ind BRAKE BACK Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(4); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin10) == HIGH)
    {
        if (!buttonPin10OFF)
        {
            buttonPin10ON = false;
            buttonPin10OFF = true;
            Serial.println("Master sending Ind BRAKE BACK Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    // Pin11 - Throttle: FORWARD PRESS
    if (digitalRead(buttonPin11) == LOW)
    {
        if (!buttonPin11ON)
        {
            buttonPin11ON = true;
            buttonPin11OFF = false;
            Serial.println("Master sending Throttle: FORWARD Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(5); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin11) == HIGH)
    {
        if (!buttonPin11OFF)
        {
            buttonPin11ON = false;
            buttonPin11OFF = true;
            Serial.println("Master sending Throttle: FORWARD Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    // Pin12 - Throttle: BACK PRESS
    if (digitalRead(buttonPin12) == LOW)
    {
        if (!buttonPin12ON)
        {
            buttonPin12ON = true;
            buttonPin12OFF = false;
            Serial.println("Master sending Throttle BACK Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(6); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin12) == HIGH)
    {
        if (!buttonPin12OFF)
        {
            buttonPin12ON = false;
            buttonPin12OFF = true;
            Serial.println("Master sending Throttle BACK Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

   /*************************************************************/
   /****Top buttons and AWS **/
   // Pin14 - AWS Button
   if (digitalRead(buttonPin14) == LOW)
    {
        if (!buttonPin14ON)
        {
            buttonPin14ON = true;
            buttonPin14OFF = false;
            Serial.println("Master sending AWS Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(21); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin14) == HIGH)
    {
        if (!buttonPin14OFF)
        {
            buttonPin14ON = false;
            buttonPin14OFF = true;
            Serial.println("Master sending AWS Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    // Pin15 - Engine_ON
    if (digitalRead(buttonPin15) == LOW)
    {
        if (!buttonPin15ON)
        {
            buttonPin15ON = true;
            buttonPin15OFF = false;
            Serial.println("Master sending Engine_ON Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(22); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin15) == HIGH)
    {
        if (!buttonPin15OFF)
        {
            buttonPin15ON = false;
            buttonPin15OFF = true;
            Serial.println("Master sending Engine_ON Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    // Pin16 - Engine_OFF
    if (digitalRead(buttonPin16) == LOW)
    {
        if (!buttonPin16ON)
        {
            buttonPin16ON = true;
            buttonPin16OFF = false;
            Serial.println("Master sending Engine_OFF Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(23); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin16) == HIGH)
    {
        if (!buttonPin16OFF)
        {
            buttonPin16ON = false;
            buttonPin16OFF = true;
            Serial.println("Master sending Engine_OFF Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    // Pin17 - Left Doors
    if (digitalRead(buttonPin17) == LOW)
    {
        if (!buttonPin17ON)
        {
            buttonPin17ON = true;
            buttonPin17OFF = false;
            Serial.println("Master sending Left Doors Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(24); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin17) == HIGH)
    {
        if (!buttonPin17OFF)
        {
            buttonPin17ON = false;
            buttonPin17OFF = true;
            Serial.println("Master sending Left Doors Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

// Pin13 -
    // int buttonStateA6 = analogRead(buttonPin20)>511?1:0;
    // Serial.println(analogRead(buttonPin20)); // DEBUG A6
    // if (!buttonStateA6) // same as HIGH
    if (digitalRead(buttonPin18) == LOW)
    {
        if (!buttonPin20ON)
        {
            buttonPin20ON = true;
            buttonPin20OFF = false;
            Serial.println("Master sending RIGHT Doors Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(25); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    // if (buttonStateA6)
    if (digitalRead(buttonPin18) == HIGH)
    {
        if (!buttonPin20OFF)
        {
            buttonPin20ON = false;
            buttonPin20OFF = true;
            Serial.println("Master sending RIGHT Doors Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    // int buttonStateA7 = analogRead(buttonPin21)>511?1:0;
    // Serial.println(analogRead(buttonPin21)); // DEBUG A6
    // if (!buttonStateA7) // same as HIGH
    if (digitalRead(buttonPin19) == LOW)
    {
        if (!buttonPin21ON)
        {
            buttonPin21ON = true;
            buttonPin21OFF = false;
            Serial.println("Master sending Door Interlock Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(26); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    // if (buttonStateA7)
    if (digitalRead(buttonPin19) == HIGH)
    {
        if (!buttonPin21OFF)
        {
            buttonPin21ON = false;
            buttonPin21OFF = true;
            Serial.println("Master sending Door Interlock Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    // REM: This button is ON in normal state.
    if (digitalRead(buttonPin22) == HIGH)
    {
        if (!buttonPin22ON)
        {
            buttonPin22ON = true;
            buttonPin22OFF = false;
            Serial.println("Master sending YELLOW-BTN Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(27); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin22) == LOW)
    {
        if (!buttonPin22OFF)
        {
            buttonPin22ON = false;
            buttonPin22OFF = true;
            Serial.println("Master sending YELLOW-BTN Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    // Pin24 - OVERRIDE
    if (digitalRead(buttonPin24) == LOW)
    {
        if (!buttonPin24ON)
        {
            buttonPin24ON = true;
            buttonPin24OFF = false;
            Serial.println("Master sending OVERRIDE Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(28); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin24) == HIGH)
    {
        if (!buttonPin24OFF)
        {
            buttonPin24ON = false;
            buttonPin24OFF = true;
            Serial.println("Master sending OVERRIDE Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    // Pin26 - ASKNOWLEDGE
    if (digitalRead(buttonPin26) == LOW)
    {
        if (!buttonPin26ON)
        {
            buttonPin26ON = true;
            buttonPin26OFF = false;
            Serial.println("Master sending ASKNOWLEDGE Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(29); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin26) == HIGH)
    {
        if (!buttonPin26OFF)
        {
            buttonPin26ON = false;
            buttonPin26OFF = true;
            Serial.println("Master sending ASKNOWLEDGE Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    // Pin28 - RELEASE
    if (digitalRead(buttonPin28) == LOW)
    {
        if (!buttonPin28ON)
        {
            buttonPin28ON = true;
            buttonPin28OFF = false;
            Serial.println("Master sending RELEASE Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(30); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin28) == HIGH)
    {
        if (!buttonPin28OFF)
        {
            buttonPin28ON = false;
            buttonPin28OFF = true;
            Serial.println("Master sending RELEASE Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    if (digitalRead(buttonPin32) == LOW)
    {
        if (!buttonPin32ON)
        {
            buttonPin32ON = true;
            buttonPin32OFF = false;
            Serial.println("Master sending Cirquit Breaker R Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(31); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin32) == HIGH)
    {
        if (!buttonPin32OFF)
        {
            buttonPin32ON = false;
            buttonPin32OFF = true;
            Serial.println("Master sending Cirquit Breaker R Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

    if (digitalRead(buttonPin34) == LOW)
    {
        if (!buttonPin34ON)
        {
            buttonPin34ON = true;
            buttonPin34OFF = false;
            Serial.println("Master sending Cirquit Breaker L Press...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(32); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }
    if (digitalRead(buttonPin34) == HIGH)
    {
        if (!buttonPin34OFF)
        {
            buttonPin34ON = false;
            buttonPin34OFF = true;
            Serial.println("Master sending Cirquit Breaker L Release...");
            Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission
            Wire.write(BTN_RELEASE); // Send a command to the slave
            Wire.endTransmission(); // End the transmission
            delay(keyPressDelay); // Debounce delay
        }
    }

}
#endif // MASTER

#ifdef SLAVE

bool flagStartOnce = true;
void setup()
{
  Wire.begin(SLAVE_ADDRESS); // Join the I2C bus as slave with address 0x08
  Wire.onReceive(receiveEvent); // Register the receive event
  Serial.begin(9600); // Start the serial monitor
  Serial.println("Slave start to listen...");
}

void loop()
{
  // Nothing to do here
  // delay(500);
  if (flagStartOnce)
  {
    Serial.println("->>>>Slave start to listen...");
    flagStartOnce = false;
  }
}

void receiveEvent(int howMany)
{
  while (Wire.available())
  {
    int command = Wire.read(); // Receive the command from the master
    Serial.println("Button pressed on master!");
    Serial.println(command);

    if (command == 21)
    {
      Serial.println("Button 21 pressed on master!");
    }
  }
}
#endif // SLAVE
