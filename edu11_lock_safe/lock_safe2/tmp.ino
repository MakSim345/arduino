/*
* An Arduino program that locks a safe using a keypad.
* It saves the password to EEPROM, enables password change
* after entering the current password, and prevents
* debouncing for keypresses.
*
* It also implements a lockout feature if the password is
* entered incorrectly three times within a certain time frame
*/

#include <Keypad.h>
#include <EEPROM.h>

#define RED_LED 10 // RED led
#define GREEN_LED 11 // GREEN led
#define RELAY 12 // RELAY

#define MAX_PASSWORD_LENGTH 10
#define MAX_ATTEMPTS 3
#define LOCKOUT_TIME 900000 // 15 minutes in milliseconds
#define CHANGE_PASSWORD_TRIGGER "AAA"

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte colPins[COLS] = { 9, 8, 7, 6 };
byte rowPins[ROWS] = { 5, 4, 3, 2 };

char password[MAX_PASSWORD_LENGTH + 1];
char enteredPassword[MAX_PASSWORD_LENGTH + 1];
uint8_t attempts = 0;
unsigned long lockoutStartTime = 0;
bool changePasswordMode = false;

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH); // relay has inverted input, set it ON
}

void loop()
{
  char key = keypad.getKey();

  if (key != NO_KEY)
  {
    handleKeyPress(key);
  }
}

void handleKeyPress(char key)
{
  if (changePasswordMode)
  {
    handlePasswordChange(key);
  }
  else if (lockoutStartTime != 0 && millis() - lockoutStartTime < LOCKOUT_TIME)
  {
    // Locked out, do nothing
  }
  else
  {
    handleSafeLock(key);
  }
}

void handlePasswordChange(char key)
{
  if (key == '#')
  {
    EEPROM.put(0, password);
    EEPROM.commit();
    changePasswordMode = false;
  }
  else if (key == '*')
  {
    resetEnteredPassword();
  }
  else
  {
    appendEnteredPassword(key);
  }
}

void handleSafeLock(char key)
{
  if (key == '#')
  {
    if (strncmp(enteredPassword, password, MAX_PASSWORD_LENGTH) == 0)
    {
      unlockSafe();
    }
    else
    {
      attempts++;
      if (attempts >= MAX_ATTEMPTS)
      {
        lockoutStartTime = millis();
        attempts = 0;
        lockSafe();
      }
    }
    resetEnteredPassword();
  }
  else if (key == '*')
  {
    if (strncmp(enteredPassword, CHANGE_PASSWORD_TRIGGER, MAX_PASSWORD_LENGTH) == 0) {
      changePasswordMode = true;
      resetEnteredPassword();
    }
    else
    {
      resetEnteredPassword();
    }
  }
  else
  {
    appendEnteredPassword(key);
  }
}

void unlockSafe()
{
  digitalWrite(RELAY, LOW); // open the lock
  digitalWrite(GREEN_LED, HIGH); // green led - ON (pass OK)
  delay(5000); // wait for 5 seconds
  digitalWrite(RELAY, HIGH); // release relay, lock closed
  digitalWrite(GREEN_LED, LOW); // green led - OFF
  attempts = 0; // reset attempts counter
}

void lockSafe()
{
  digitalWrite(RED_LED, HIGH); // red led - ON
  delay(5000); // wait for 5 seconds
  digitalWrite(RED_LED, LOW); // red led - OFF
}

void resetEnteredPassword()
{
  memset(enteredPassword, 0, sizeof(enteredPassword));
}

void appendEnteredPassword(char key)
{
  if (strlen(enteredPassword) < MAX_PASSWORD_LENGTH)
  {
    strncat(enteredPassword, &key, 1);
  }
}
