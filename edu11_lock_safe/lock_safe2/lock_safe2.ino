#include <Keypad.h>

#define RED_LED 10 // RED led
#define GREEN_LED 11 // GREEN led
#define RELAY 12 // RELAY

#define NUM_KEYS 5 // code counter

char pressedKey;
char aSecretKey[NUM_KEYS] = { '1', '2', '3', '4', '5' }; // secret key in array
char aKeyPressed[NUM_KEYS];
int keyPressCounter = 0;
int keyCounterMatch = 0;

const byte ROWS = 4;
const byte COLS = 4;

char aKeysOnKeyPad[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

byte rowPins[ROWS] = { 5, 4, 3, 2 };
byte colPins[COLS] = { 9, 8, 7, 6 };


// Create a keypad object:
Keypad safeLockKeypad = Keypad(makeKeymap(aKeysOnKeyPad), rowPins, colPins, ROWS, COLS); 

void setup()
{
    pinMode(RED_LED, OUTPUT); 
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RELAY, OUTPUT);
    digitalWrite(RELAY, HIGH); // note: relay has inverted input, set it ON    
}

void loop()
{
    pressedKey = safeLockKeypad.getKey(); // scanning Keypad to check if any key pressed

    if (pressedKey != NO_KEY) // a key pressed
    {
        aKeyPressed[keyPressCounter] = pressedKey; // save pressed key to an array
        keyPressCounter = keyPressCounter + 1;     // increment pressed keys counter to remember

        if (keyPressCounter == NUM_KEYS) // pressed keys are same as the code lenght
        {
            for (uint8_t i = 0; i < NUM_KEYS; i++) // scan over the array of input codes
            {
                if (aKeyPressed[i] == aSecretKey[i])
                {
                    keyCounterMatch = keyCounterMatch + 1; // incremet match counter
                }
            }

            if (keyCounterMatch == NUM_KEYS) // code match
            {
                digitalWrite(RELAY, LOW); // open the lock
                digitalWrite(GREEN_LED, HIGH); // green led - ON, pass OK
                delay(5000); // wait for 5 seconds
                digitalWrite(RELAY, HIGH); // release relay, lock closed
                digitalWrite(GREEN_LED, LOW); // green led - OFF

                // reset all counters    
                keyPressCounter = 0;
                keyCounterMatch = 0;
            }
            else
            {
                // not all keys matched
                digitalWrite(RED_LED, HIGH); // red led - ON
                delay(5000); // wait for 5 seconds
                digitalWrite(RED_LED, LOW); // red led - OFF
                // reset all counters    
                keyPressCounter = 0;
                keyCounterMatch = 0;
            }
        }
    }
}