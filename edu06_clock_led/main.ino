/*
  Blink Morse code
  Turns on an LED in SOS mode, repeatedly.
 */
 
#define LED_PIN 13
#define LED_PIN_GREEN 12

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
int catodos[] = {12};

int value[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int currentDigit = -1;
long nextChange;


const int NUM_DIGITS = 10;
const int DIGIT_DELAY = 1000; // 2ms por digito

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
  // Coloca todos os catodos em alta impedancia para nao conduzir
  for (int c = 0; c < 4; c++)
    pinMode(catodos[c], INPUT);
}

void loop() 
{
 long time = millis();
  if (time >= nextChange) 
  {
    // Roda o digito atual
    currentDigit = (currentDigit + 1) % NUM_DIGITS;
    //currentDigit = 3;
    Serial.println(currentDigit);
    //reset(); // Apaga tudo antes de escrever o proximo algarismo

    writeNumber(value[currentDigit]); // Escreve o proximo algarismo
    // writeNumber(2);
    // Ativa apenas no catodo referente ao digito
    //pinMode(catodos[currentDigit], OUTPUT);
    pinMode(12, OUTPUT);
    //digitalWrite(catodos[currentDigit], LOW);
    digitalWrite(12, LOW);

    // Marca o tempo para a proxima troca
    nextChange = time + DIGIT_DELAY;
  }
}

void loop_non_use() 
{
  long time = millis();
  if (time >= nextChange) 
  {
    // Roda o digito atual
    currentDigit = (currentDigit + 1) % NUM_DIGITS;

    Serial.println(currentDigit);
    //reset(); // Apaga tudo antes de escrever o proximo algarismo

    writeNumber(value[currentDigit]); // Escreve o proximo algarismo

    // Ativa apenas no catodo referente ao digito
    pinMode(catodos[currentDigit], OUTPUT);
    
    digitalWrite(catodos[currentDigit], LOW);

    // Marca o tempo para a proxima troca
    nextChange = time + DIGIT_DELAY;
  }

}

void writeNumber(int num) 
{
  for (int a = 0; a < 8; a++) 
    {
        if (DIGITS[num][a] == 1)
            digitalWrite(anodos[a], HIGH);
        else
            digitalWrite(anodos[a], LOW);
    }
}
