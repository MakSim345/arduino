#include <Servo.h> 

#define SERVO_PIN 9
#define MAX_DEGREE 100
#define LED_PIN 14
#define MIN_SECONDS_DELAY 2000
#define MAX_SECONDS_DELAY 9000
#define SET_TARGET_DELAY 2000

// Let's create servo object to control a servo 
// ATTN: a maximum of eight servo objects can be created 
Servo myservo;  
// 544 - this is standard impulse width when servo shall go to 0
#define servoMinImp 544 
// 2400 this is standard impulse width when servo shall go to 180
#define servoMaxImp 2400 

int min_pos = 14;    // variable to store the servo position 
int pos = 0;
const int _delay_servo = 10;
long randOn = 0; 
boolean flag = false;
 
void setup() 
{ 
    myservo.attach(SERVO_PIN, servoMinImp, servoMaxImp);  // attaches the servo on pin 9 to the servo object 
    randomSeed (analogRead (0));    // randomize 
    pinMode(LED_PIN, OUTPUT); 
} 
 
 
void loop() 
{ 
    if (flag == false)
    {
        flag = true;
        digitalWrite(LED_PIN, LOW); 
        myservo.write(min_pos);
    }
    else
    {
        randOn = random (MIN_SECONDS_DELAY, MAX_SECONDS_DELAY); // generate ON time between 1 and 2 seconds
  
        digitalWrite(LED_PIN, HIGH);   // sets the LED ON
           
        myservo.write(min_pos + 99);               
        delay(_delay_servo);
      
        delay(SET_TARGET_DELAY);
      
        myservo.write(min_pos);               // tell servo to go to position in variable 'pos' 
        delay(_delay_servo);                  // waits 15ms for the servo to reach the position 
        
        digitalWrite(LED_PIN, LOW);   // sets the LED off
        delay(randOn);
    }

} 

