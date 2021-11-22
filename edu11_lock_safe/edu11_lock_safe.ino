/* 
 * origin: 
 * http://musicdiver.com/wordpress/2013/01/keypad-code-lock-with-arduino/

  * The program has a bug. 
  * Can you find it? 
  * What happens if you input 23571 23571 ? Why?
*/

#include <Keypad.h>

char* secretCode = "235711";
int position = 0;

const byte rows = 4;
const byte cols = 3;
char keys[rows][cols] = 
{
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[rows] = {4, 5, 6, 7};
byte colPins[cols] = {1, 2, 3};

Keypad keypad = Keypad(makeKeymap(keys), 
                       rowPins, colPins,
                       rows, cols);

int redPin = 8;
int greenPin = 9;

void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  setLocked(true);
}

void loop()
{
  // char key = keypad.getKey();
  
  if (key == '*' || key == '#') 
  {
    position = 0;
    setLocked(true);
  }
  
  if (key == secretCode[position]) 
  {
    position++;
  }
  
  if (position == 6) 
  {
    setLocked(false);
  }
  delay(50);
}

void setLocked(int locked)
{
  if (locked) 
  {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
  }
  else 
  {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
  }
}

