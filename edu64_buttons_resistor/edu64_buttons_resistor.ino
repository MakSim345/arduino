//const int analogPin = A0;
const int analogPin = A1;
//const int analogPin = A2;

// Measured values for each button
/*
const int btn6Value = 828;
const int btn5Value = 795;
const int btn4Value = 747;
const int btn3Value = 667;
const int btn2Value = 505;
const int btn1Value = 27;
const int noPressValue = 940;
*/

const int btn6Value = 860;
const int btn5Value = 810;
const int btn4Value = 780;
const int btn3Value = 701;
const int btn2Value = 533;
const int btn1Value = 50;
const int noPressValue = 1020;

const int delayValue = 100;

const int resRange = 20;

void setup()
{
  Serial.begin(9600);
}

void printMeasuredValues()
{
  int analogValue = analogRead(analogPin);
  Serial.println(analogValue);

  delay(500);  // Debounce delay
}

void main_loop()
{
  int analogValue = analogRead(analogPin);
  // Serial.println(analogValue);

   /* Checks the analog value against the measured values for each button.
    * If a match is found within a small range (+-resRange), it sends
    * the corresponding keystroke
    */
  if (analogValue >= btn1Value - resRange && analogValue <= btn1Value + resRange)
  {
    Serial.println(analogValue);
    Serial.println("press('a')");  // Button 1§
    Serial.println("release('a')");
  }
  else if (analogValue >= btn2Value - resRange && analogValue <= btn2Value + resRange)
  {
    Serial.println(analogValue);
    Serial.println("press('b')");  // Button 1
    delay(delayValue);
    Serial.println("release('b')");
  }
  else if (analogValue >= btn3Value - resRange && analogValue <= btn3Value + resRange)
  {
    Serial.println(analogValue);
    Serial.println("press('c')");  // Button 1
    delay(delayValue);
    Serial.println("release('c')");
  }
  else if (analogValue >= btn4Value - resRange && analogValue <= btn4Value + resRange)
  {
    Serial.println(analogValue);
    Serial.println("press('d')");  // Button 1
    delay(delayValue);
    Serial.println("release('d')");
  }
  else if (analogValue >= btn5Value - resRange && analogValue <= btn5Value + resRange)
  {
    Serial.println(analogValue);
    Serial.println("press('e')");  // Button 1
    delay(delayValue);
    Serial.println("release('e')");
  }
  else if (analogValue >= btn6Value - resRange && analogValue <= btn6Value + resRange)
  {
    Serial.println(analogValue);
    Serial.println("press('f')");  // Button 1
    delay(delayValue);
    Serial.println("release('f')");
  }

  else if (analogValue >= noPressValue - resRange && analogValue <= noPressValue + resRange)
  {
    // No button pressed
  }

  delay(100);  // Debounce delay
}

void loop()
{
    //printMeasuredValues();
    main_loop();
}
