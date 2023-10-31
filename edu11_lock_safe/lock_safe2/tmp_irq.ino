/*An example of an Arduino program that uses a keypad to lock and unlock a safe. It utilizes interrupts to read the key, providing a more responsive and efficient way of capturing keypad input
*/

#include <Keypad.h>
#include <EEPROM.h>

const int PASSWORD_LENGTH = 10;
const int PASSWORD_ADDRESS = 0;

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = 
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

volatile bool keyEntered = false;
volatile char enteredKey;

void setup() 
{
  Serial.begin(9600);
  initializePassword();
  attachInterrupt(digitalPinToInterrupt(2), readKey, FALLING);
}

void loop() 
{
  if (keyEntered) 
  {
    if (enteredKey == '#') 
    {
      unlockSafe();
    }
    else 
    {
      enterPassword(enteredKey);
    }
    keyEntered = false;
  }
}

void initializePassword() 
{
  char password[PASSWORD_LENGTH + 1];
  EEPROM.get(PASSWORD_ADDRESS, password);

  if (password[0] == '\0') 
  {
    Serial.println("Please enter a new password:");

    while (true) 
    {
      if (keyEntered && enteredKey != '#') 
      {
        int passwordLength = strlen(password);
        if (passwordLength < PASSWORD_LENGTH) 
        {
          password[passwordLength] = enteredKey;
          password[passwordLength + 1] = '\0';
        }
        keyEntered = false;
      } 
      else if (enteredKey == '#') 
      {
        EEPROM.put(PASSWORD_ADDRESS, password);
        Serial.println("Password saved.");
        break;
      }
    }
  } 
  else 
  {
    Serial.println("Password already set.");
  }
}

void enterPassword(char key) 
{
  static char enteredPassword[PASSWORD_LENGTH + 1];
  static int passwordIndex = 0;

  if (passwordIndex < PASSWORD_LENGTH) 
  {
    enteredPassword[passwordIndex++] = key;
    Serial.print("*");
  }

  if (passwordIndex == PASSWORD_LENGTH) 
  {
    enteredPassword[passwordIndex] = '\0';
    Serial.println();

    char password[PASSWORD_LENGTH + 1];
    EEPROM.get(PASSWORD_ADDRESS, password);

    if (strcmp(enteredPassword, password) == 0) 
    {
      Serial.println("Safe unlocked.");
      // Add your code for unlocking the safe here
    }
    else 
    {
      Serial.println("Wrong password. Try again.");
    }

    passwordIndex = 0;
    memset(enteredPassword, 0, sizeof(enteredPassword));
  }
}

void unlockSafe() 
{
  // Add your code for unlocking the safe here
}

void readKey() 
{
  char key = keypad.getKey();
  if (key) 
  {
    keyEntered = true;
    enteredKey = key;
  }
}

/*
In this code, interrupts are used to read the keypad input. The `attachInterrupt` function is called to set up an interrupt on pin 2, which triggers the `readKey` function whenever a falling edge is detected. The `readKey` function captures the key and sets the `keyEntered` flag to true, indicating that a key has been entered.

The main `loop` function checks the `keyEntered` flag and processes the entered key by calling the respective functions for initializing the password, entering the password, or unlocking the safe.

Please note that the interrupt pin used in this example is pin 2, but you can modify it according to your specific setup. Additionally, ensure that your Arduino board supports interrupts on the chosen pin.

Remember to adapt and enhance the code as per your requirements and add the appropriate code for unlocking the safe within the `unlockSafe` function.
*/
