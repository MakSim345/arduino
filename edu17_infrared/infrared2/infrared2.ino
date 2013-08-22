#include <Servo.h> 

/* 
 * //////////////////////////////////////////////////
 * //making sense of the Parallax PIR sensor's output
 * //////////////////////////////////////////////////
 * 
 * Switches a LED according to the state of the sensors output pin.
 * Determines the beginning and end of continuous motion sequences.
 *
 * released under a creative commons "Attribution-NonCommercial-ShareAlike 2.0" license
 *
 * The Parallax PIR Sensor is an easy to use digital infrared motion sensor module. 
 *
 * The sensor's output pin goes to HIGH if motion is present.
 * However, even if motion is present it goes to LOW from time to time, 
 * which might give the impression no motion is present. 
 * This program deals with this issue by ignoring LOW-phases shorter than a given time, 
 * assuming continuous motion is present during these phases.
 *  
 */


/////////////////////////////
//VARS
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)

#define SERVO_PIN 3
#define MAX_DEGREE 100
#define LED_PIN 14
#define MIN_SECONDS_DELAY 2000
#define MAX_SECONDS_DELAY 9000
#define SET_TARGET_DELAY 2000
#define LED_BLINK_COUNT 20

// Let's create servo object to control a servo 
// ATTN: a maximum of eight servo objects can be created 
Servo myservo;       

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         
int calibrationTime = 5;   

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 100;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 7;    //the digital pin connected to the PIR sensor's output
int ledPin = 12;
int min_pos = 1;    // variable to store the servo position 
int pos = 0;
const int _delay_servo = 10;
boolean set_led_blink = false;
int led_blink_ctr = LED_BLINK_COUNT;
int ledState;
unsigned long currentMillis, previousMillis;
long interval = 80;           // interval at which to blink (milliseconds)

void my_printf(char *);

//SETUP
void setup()
{
    myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object 
    pinMode(SERVO_PIN, OUTPUT); 
    myservo.write(min_pos);

    Serial.begin(9600);
    pinMode(pirPin, INPUT);
    pinMode(ledPin, OUTPUT);
    digitalWrite(pirPin, LOW);

    //give the sensor some time to calibrate
    Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++)
    {
        Serial.print(".");
        digitalWrite(ledPin, HIGH);
        delay(500);
        digitalWrite(ledPin, LOW);
        delay(500);
    }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
}

//LOOP
void loop()
{
    currentMillis = millis();
    // interval = 80 ms;
    if(currentMillis - previousMillis > interval) 
    {
        // save the last time you blinked the LED
        previousMillis = currentMillis;
        if (set_led_blink)
        {
            led_blink_ctr = led_blink_ctr - 1;
            if (led_blink_ctr <= 0)
            {
                led_blink_ctr = LED_BLINK_COUNT;
                set_led_blink = false;
                // my_printf ("STOP blinking!");
            }

            // if the LED is off turn it on and vice-versa:
            if (ledState == LOW)
                ledState = HIGH;
            else
                ledState = LOW;

            // set the LED with the ledState of the variable:
            digitalWrite(ledPin, ledState);
        }
    }
    
    if(digitalRead(pirPin) == HIGH)
    {
        // my_printf("Sensor = HIGH");
        // digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
        set_led_blink = true;

        if(lockLow)
        {  
            myservo.write(min_pos + 40);               
            delay(_delay_servo);
            //makes sure we wait for a transition to LOW before any further output is made:
            lockLow = false;            
            Serial.println("---");
            Serial.print("motion detected at ");
            Serial.print(millis()/1000);
            Serial.println(" sec"); 
            delay(50);
        }         
        takeLowTime = true;
     }

     if(digitalRead(pirPin) == LOW)
     {       
       // digitalWrite(ledPin, LOW);  //Led off
       // set_led_blink = false;

       if(takeLowTime)
       {
          lowIn = millis();          //save the time of the transition from high to LOW
          takeLowTime = false;       //make sure this is only done at the start of a LOW phase
       }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && (millis() - lowIn) > pause)
       {  
           
           myservo.write(min_pos);               // tell servo to go to position in variable 'pos' 
           delay(_delay_servo);                  // waits 15ms for the servo to reach the position 

           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           delay(50);
       }
     }
  }


void my_printf(char * _text)
{
    Serial.println("---");
    Serial.println(_text); 
    delay(50);

}
