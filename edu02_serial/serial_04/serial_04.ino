/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
#define BOOTLOADER_TEST_PIN_07 7
// give it a name:
int ledPin = BOOTLOADER_TEST_PIN_07; //13;
int ledGreenPin = 12;
int longWait = 500;
int shortWait = 200;
long randOn = 0;                  // Initialize a variable for the ON time
long randOff = 0;                 // Initialize a variable for the OFF time


// the setup routine runs once when you press reset:
void setup() 
{
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT); 
  pinMode(ledGreenPin, OUTPUT); 
  randomSeed (analogRead (0));    // randomize 
  Serial.begin(9600);   
}

void random_leds()
{
  
  randOn = random (100, 1200);    // generate ON time between 0.1 and 1.2 seconds
  randOff = random (200, 900);    // generate OFF time between 0.2 and 0.9 seconds
  
  Serial.write("ok\n"); // 
  char v_str[8] = "       ";  //reserve the string space first
  itoa(randOn, v_str, 6);
  // lcd.printIn(v_str);
  int bytesSent = Serial.write(v_str); //send the string “hello” and return the length of the string.
  Serial.write("--\n"); 

  digitalWrite(ledPin, HIGH);   // sets the LED on
  digitalWrite(ledGreenPin, LOW);
  delay(randOn);                // waits for a random time while ON
  
  digitalWrite(ledPin, LOW);    // sets the LED off
  digitalWrite(ledGreenPin, HIGH);
  delay(randOff);               // waits for a random time while OFF

  
}

// the loop routine runs over and over again forever:
void loop() 
{
  // Serial.write("App start.\n");
  random_leds();
/*
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(longWait);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(longWait);               // wait for a second
  */

  /*
  digitalWrite(led, HIGH);   
  delay(longWait);               
  
  digitalWrite(led, LOW); 
  delay(longWait);               
  digitalWrite(led, HIGH);   
  delay(longWait);               
  digitalWrite(led, LOW); 
  delay(longWait);               
  digitalWrite(led, HIGH);
  delay(shortWait);
  digitalWrite(led, LOW);
  delay(shortWait);
  digitalWrite(led, HIGH);
  delay(shortWait);
  digitalWrite(led, LOW);
  delay(shortWait);
  digitalWrite(led, HIGH);
  delay(shortWait);
  digitalWrite(led, LOW);
  delay(longWait); 
  */
}
