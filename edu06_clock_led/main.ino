/*
  Blink Morse code
  Turns on an LED in SOS mode, repeatedly.
 */
 
#define LED_PIN 13
#define LED_PIN_GREEN 12
#define DISPLAY_BRIGHTNESS  1000

#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW
#define ERASE_SEGMENTS 10

void writeNumber(int num); 

const int E = 10; // 2
const int D = 3;
const int H = 4;
const int C = 5;
const int G = 6;
const int B = 7;
const int F = 8;
const int A = 9;


int anodos[] = { A, B, C, D, E, F, G, H };
// int catodos[] = { 10, 11, 12, 13 };
int catodos[] = {12, 13};

int value[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int currentDigit = -1;
int firdstDigit = -1;
int secondDigit = -1;
long nextChange;


const int NUM_DIGITS = 100;
const int DIGIT_DELAY = 10; // 2ms por digito

const byte DIGITS[10][8] = {
//A  B  C  D  E  F  G  H
{ 1, 1, 1, 1, 1, 1, 0, 0 }, // 0
{ 0, 1, 1, 0, 0, 0, 0, 0 }, // 1
{ 1, 1, 0, 1, 0, 1, 1, 0 }, // 2
{ 1, 1, 1, 1, 0, 0, 1, 0 }, // 3 
{ 0, 1, 1, 0, 1, 0, 1, 0 }, // 4
{ 1, 0, 1, 1, 1, 0, 1, 0 }, // 5
{ 1, 0, 1, 1, 1, 1, 1, 0 }, // 6
{ 1, 1, 1, 0, 0, 0, 0, 0 }, // 7
{ 1, 1, 1, 1, 1, 1, 1, 0 }, // 8
{ 1, 1, 1, 1, 1, 0, 1, 0 }  // 9
};
  
void setup() 
{
  for (int a = 0; a < 8; a++)
    pinMode(anodos[a], OUTPUT);

  reset();

  nextChange = 0;

  Serial.begin(9600);
}

void reset() 
{
  const int cat_num = 2; // number of catodos
  for (int c = 0; c < cat_num; c++)
   {
    //pinMode(catodos[c], INPUT);
    pinMode(catodos[c], OUTPUT);
   }
}

void loop_ver_3()
{
    //long startTime = millis();
    displayNumber(millis()/1000);
}

void loop() 
{
    long time = millis();
    if (time >= nextChange) 
    {
        // currentDigit = (currentDigit + 1) % NUM_DIGITS;
        currentDigit = (millis()/1000) % NUM_DIGITS;
        firdstDigit = (currentDigit - currentDigit % 10)/10;
        secondDigit = currentDigit % 10;

        //currentDigit = 3;
        /*
        Serial.println(currentDigit);
        Serial.println(firdstDigit);
        Serial.println(secondDigit);
        Serial.println("....");
        */
        for(int digit = 2; digit > 0 ; digit--) 
        {
        //Turn on a digit for a short amount of time
        switch(digit) 
            {
            case 1:
                digitalWrite(12, DIGIT_ON);
                // writeNumber(value[secondDigit], 12); // Escreve o proximo algarismo
                writeNumber(firdstDigit);
                delayMicroseconds(DISPLAY_BRIGHTNESS);
                writeNumber(ERASE_SEGMENTS);
                digitalWrite(12, DIGIT_OFF);
                break;
            case 2:
                digitalWrite(13, DIGIT_ON);
                // writeNumber(value[secondDigit], 13); // Escreve o proximo algarismo
                writeNumber(secondDigit);
                delayMicroseconds(DISPLAY_BRIGHTNESS); 
                writeNumber(ERASE_SEGMENTS);
                digitalWrite(13, DIGIT_OFF);
                break;
            }
        }
        nextChange = time + DIGIT_DELAY;
    }
}

void loop_non_use() 
{
    long time = millis();
    if (time >= nextChange) 
    {
        currentDigit = (currentDigit + 1) % NUM_DIGITS;
        Serial.println(currentDigit);
        //reset(); // 
        writeNumber(value[currentDigit]); //
        pinMode(catodos[currentDigit], OUTPUT);    
        digitalWrite(catodos[currentDigit], LOW);
        nextChange = time + DIGIT_DELAY;
    }
}

void writeNumber(int num, int catod) 
{
  // reset(); // Apaga tudo antes de escrever o proximo algarismo
  
  for (int a = 0; a < 8; a++) 
    {
        if (DIGITS[num][a] == 1)
            digitalWrite(anodos[a], HIGH);
        else
            digitalWrite(anodos[a], LOW);
    }

  pinMode(catod, OUTPUT);    
  digitalWrite(catod, LOW);
}

void writeNumber(int num) 
{
    if (num > 9)
    {
      for (int a = 0; a < 8; a++) 
       {
        digitalWrite(anodos[a], LOW);
       }
    }
    
    for (int a = 0; a < 8; a++) 
    {
        if (DIGITS[num][a] == 1)
            digitalWrite(anodos[a], HIGH);
        else
            digitalWrite(anodos[a], LOW);
    }
}

//Given a number, we display 10:22
//After running through the 4 numbers, the display is left turned off

//Display brightness
//Each digit is on for a certain amount of microseconds
//Then it is off until we have reached a total of 20ms for the function call
//Let's assume each digit is on for 1000us
//Each digit is on for 1ms, there are 4 digits, so the display is off for 16ms.
//That's a ratio of 1ms to 16ms or 6.25% on time (PWM).
//Let's define a variable called brightness that varies from:
//5000 blindingly bright (15.7mA current draw per digit)
//2000 shockingly bright (11.4mA current draw per digit)
//1000 pretty bright (5.9mA)
//500 normal (3mA)
//200 dim but readable (1.4mA)
//50 dim but readable (0.56mA)
//5 dim but readable (0.31mA)
//1 dim but readable in dark (0.28mA)

void displayNumber(int toDisplay)
{
/*
int digit1 = 11; //PWM Display pin 1
int digit2 = 10; //PWM Display pin 2
int digit3 = 9; //PWM Display pin 6
int digit4 = 6; //PWM Display pin 8
*/
const int digit_number = 2; // must be 4 in release.

int digit1 = 12; //PWM Display pin 1
int digit2 = 13; //PWM Display pin 2
int digit3 = 12; //PWM Display pin 6
int digit4 = 13; //PWM Display pin 8

  long beginTime = millis();

  for(int digit = digit_number ; digit > 0 ; digit--) 
  {
    //Turn on a digit for a short amount of time
    switch(digit) 
    {
    case 1:
      digitalWrite(digit1, DIGIT_ON);
      break;
    case 2:
      digitalWrite(digit2, DIGIT_ON);
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
      break;
    }

    //Turn on the right segments for this digit
    // lightNumber(toDisplay % 10);
    writeNumber(toDisplay % 10);
    toDisplay /= 10;

    delayMicroseconds(DISPLAY_BRIGHTNESS); 
    //Display digit for fraction of a second (1us to 5000us, 500 is pretty good)

    //Turn off all segments
    // lightNumber(10); 
    writeNumber(10);

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
  }

  //Wait for 20ms to pass before we paint the display again:
  while( (millis() - beginTime) < 10) ; 
}

