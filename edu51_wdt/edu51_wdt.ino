#include <avr/wdt.h> //should be in any adruino IDE

// #define RESET_WDT
int resetPIN=12;
int _ctr = 0;
// the setup routine runs once when you press reset:
void setup() 
{  
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


// the loop routine runs over and over again forever:
void loop() 
{
#ifdef RESET_WDT
  // wdt_reset();  
#endif
  Serial.println("Arduino running");
  delay (1000);
  _ctr++;
  if (_ctr >= 8)
  {
    digitalWrite(resetPIN, LOW);
  }
}
