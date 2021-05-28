/*
S.O.S morse code all LEDs!
The Hello World for the distressed chip
*/

int S = 1;
int O = 2;

#define DOT_DELAY 130
#define DASH_DELAY 250
#define STOP_DELAY 500
#define MAX_PIN_NUMBER 14

void dot()
{
  for(int j=0; j<MAX_PIN_NUMBER; j++)
     {
        digitalWrite(j, HIGH);
     }     
    delay(DOT_DELAY); //
  for(int j=0; j<MAX_PIN_NUMBER; j++)
     {
        digitalWrite(j, LOW);
     }     
    delay(DOT_DELAY); //
}

void dash()
{
  for(int j=0; j<MAX_PIN_NUMBER; j++)
     {
        digitalWrite(j, HIGH);
     }     
    delay(DASH_DELAY); //

  for(int j=0; j<MAX_PIN_NUMBER; j++)
     {
        digitalWrite(j, LOW);
     }     
    delay(DASH_DELAY); //
}

void flash(int pLetter)
{
  switch (pLetter) 
  {
    case 1: // the letter 'S'
        dot();
        dot();
        dot();
      break;
    case 2: // the letter 'O'
        dash();
        dash();
        dash();
      break;
    default:
      Serial.println("Error in flash");
      // do something
  }
  delay(250); // break between each letter
}


// the setup routine runs once when you press reset:
void setup() 
{
    for(int i=0; i<MAX_PIN_NUMBER; i++)
     {
        pinMode(i, OUTPUT);
     }     
    Serial.begin(9600); 
    Serial.println("Arduino: Setup OK. Running...");
}

// the loop routine runs over and over again forever:
void loop() 
{
  // Serial.println("Arduino running");
  
  flash(S);
  flash(O);
  flash(S);

  Serial.println("Arduino: Running...");
  delay (STOP_DELAY);
}



/*
  digitalWrite(resetPIN, HIGH);
  pinMode(resetPIN, OUTPUT);

  Serial.begin(9600);   
  Serial.println("");
  Serial.println("------- Arduino Rebooted");
  Serial.println("");
  // wdt_disable(); //always good to disable it, if it was left 'on' or you need init time
  // do some stuff here
  // wdt_enable(WDTO_8S); //enable it, and set it to 8s
}

*/
