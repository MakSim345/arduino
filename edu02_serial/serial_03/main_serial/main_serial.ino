/* Simple Serial ECHO script */

/* Use a variable called byteRead to temporarily store
   the data coming from the computer */
byte byteRead;
const unsigned int LED_PIN =  13;
long interval = 1000;           // interval at which to blink (milliseconds)

// Variables will change:
int ledState = LOW;             // ledState used to set the LED

void setup() 
{                
  // Turn the Serial Protocol ON
  Serial.begin(9600);
  // set the digital pin as output:
  pinMode(LED_PIN, OUTPUT);

}

void loop() 
{
   /*  check if data has been sent from the computer: */
  if (Serial.available()) 
  {
    /* read the most recent byte */
    byteRead = Serial.read();
    /*ECHO the value that was read, back to the serial port. */
    //Serial.write(byteRead);
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
  }

  // set the LED with the ledState of the variable:
  digitalWrite(LED_PIN, ledState);

}
