/*
 * origin:
 * http://musicdiver.com/wordpress/2013/01/keypad-code-lock-with-arduino/

  * The program has a bug.
  * Can you find it?
  * What happens if you input 23571 23571 ? Why?
*/

// #include <Keypad.h>

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

/*
Keypad keypad = Keypad(makeKeymap(keys),
                       rowPins, colPins,
                       rows, cols);
*/

int redPin = 8;
int greenPin = 13; // use 13 led to show opened door.

char* secretCode = "235711";
int position = 0;

void setup()
{
  pinMode(redPin, OUTPUT);

  pinMode(greenPin, OUTPUT);
  digitalWrite(greenPin, LOW);

  setLocked(true);
  Serial.begin(9600);
  Serial.println("App started  >------------------>");
}


void checkTheCode()
{
  char key;
  // char* hacker = "2357123571";
  char* hacker = "235711";

  for (int i = 0; i < 10; i++)
  {
    key = hacker[i];
    Serial.write("Key - ");
    Serial.write(key);
    Serial.write("\n");
    delay(500);

    if (key == secretCode[position])
    {
        position++;
        Serial.write("position - ");
        Serial.println(position);
    }
    else // fix the bug: position reset if wrong digit appears
    {
        position = 0;
        setLocked(true);
    }

    if (key == '*' || key == '#')
    {
        position = 0;
        setLocked(true);
    }

    if (position == 6)
    {
        setLocked(false);
        Serial.println("Open the door!");
    }
  }
}

void loop()
{
  // char key = keypad.getKey();
  // char key;
  static bool firstTime = true;

  if (firstTime)
  {
    firstTime = false;
    checkTheCode();
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

