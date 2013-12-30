// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.

#include <Servo.h> 

#define SERVO_PIN 12
#define MAX_DEGREE 90
#define LED_PIN 13
#define MIN_SECONDS_DELAY 2000
#define MAX_SECONDS_DELAY 9000
#define SET_TARGET_DELAY 1000

// Let's create servo object to control a servo 
// ATTN: a maximum of eight servo objects can be created 
Servo myservo;  

int pos = 0;    // variable to store the servo position 
const int _delay_servo = 2;
long randOn = 0; 
 
void setup() 
{ 
    myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object 
    randomSeed (analogRead (0));    // randomize 
    pinMode(LED_PIN, OUTPUT); 
} 
 
 
void loop() 
{ 
    randOn = random (MIN_SECONDS_DELAY, MAX_SECONDS_DELAY); // generate ON time between 1 and 2 seconds
  
    digitalWrite(LED_PIN, HIGH);   // sets the LED on
  
    for(pos = 0; pos < MAX_DEGREE; pos += 1)  // goes from 0 degrees to 180 degrees 
    {                                     // in steps of 1 degree 
        myservo.write(pos);               // tell servo to go to position in variable 'pos' 
        delay(_delay_servo);              // waits 15ms for the servo to reach the position 
    } 
  
    delay(SET_TARGET_DELAY);
  
    for(pos = MAX_DEGREE; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
    {                                
        myservo.write(pos);              // tell servo to go to position in variable 'pos' 
        delay(_delay_servo);                       // waits 15ms for the servo to reach the position 
    } 
    digitalWrite(LED_PIN, LOW);   // sets the LED on
    delay(randOn);
} 

