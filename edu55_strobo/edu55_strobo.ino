/* Blink without Delay

   Turns on and off a light emitting diode(LED) connected to a digital
   pin, without using the delay() function.  This means that other code
   can run at the same time without being interrupted by the LED code.

   The circuit:
 * LED attached from pin 13 to ground.
 * Note: on most Arduinos, there is already an LED on the board
 that's attached to pin 13, so no hardware is needed for this example.


http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
 */

#define GREEN_PIN 4
#define BLUE_PIN 2
#define POT_PIN 3

#define BRIGHTNESS HIGH

#define FREQUENCY_MIN 30
#define FREQUENCY_MAX 120

#define DUTY_CYCLE 0.2

#define ON 1
#define OFF 0

int state;
float frequency;
int val;

// #define USE_EMBEDDED_LED
#define USE_EXTERNAL_LED

#ifdef USE_EXTERNAL_LED
    // const unsigned int LED_PIN = A0;
    #define RED_PIN 8
    // #define RED_PIN 13 // used to be 11, but change to embedded
    // #define LED_STROBO_01 7
    #define LED_STROBO_01 12
#endif

/*
 Arduino NANO, pin #XX
*/
#ifdef USE_EMBEDDED_LED
    // const unsigned int LED_PIN = 11;
    #define RED_PIN 10
    #define LED_STROBO_01 13 // EMBEDDED LED
#endif


// const unsigned int LED_PIN = A0;
// Variables will change:
int ledState = LOW;             // ledState used to set the LED
bool strobState = false;
long previousMillis = 0;        // will store last time LED was updated
long previousStrobMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.

/*
#define LED_ON_INTERVAL 20
#define LED_OFF_INTERVAL 80
#define STROB_ON_INTERVAL 350
#define STROB_OFF_INTERVAL 250
*/
#define LED_ON_INTERVAL 10
#define LED_OFF_INTERVAL 45

//#define LED_ON_INTERVAL 15
//#define LED_OFF_INTERVAL 80

#define STROB_ON_INTERVAL (LED_ON_INTERVAL+LED_OFF_INTERVAL)*6
#define STROB_OFF_INTERVAL 180

long blinkInterval = LED_ON_INTERVAL;    // interval at which to blink (milliseconds)
long strobInterval = STROB_ON_INTERVAL;

unsigned long currentMillis;

void setup()
{
    // set the digital pin as output:
    pinMode(RED_PIN, OUTPUT);
    pinMode(LED_STROBO_01, OUTPUT);
    // Serial.begin(9600);
}

void loop()
{
    unsigned long strobMillis = millis();

    if(strobMillis - previousStrobMillis > strobInterval)
    {
        strobState = !strobState;
        previousStrobMillis = strobMillis;

        if (strobState)
        {
            strobInterval = STROB_ON_INTERVAL;
        }
        else
        {
            strobInterval = STROB_OFF_INTERVAL;
        }
        // Serial.println("state = OFF");
    }

    if (strobState)
    {
        currentMillis = millis();
        if (currentMillis - previousMillis > blinkInterval)
        {
            // save the last time you blinked the LED
            previousMillis = currentMillis;
            // if the LED is off turn it on and vice-versa:
            if (ledState == HIGH)
            {
                ledState = LOW;
                blinkInterval = LED_OFF_INTERVAL;
            }
            else
            {
                ledState = HIGH;
                blinkInterval = LED_ON_INTERVAL;
            }

            digitalWrite(RED_PIN, ledState);
            digitalWrite(LED_STROBO_01, ledState);
        }
    }
    else // all leds off
    {
        digitalWrite(RED_PIN, LOW);
        digitalWrite(LED_STROBO_01, LOW);
    }
}


/*
* TODO: remove old code *

void loop_bkp()
{
    val = 2; // analogRead(POT_PIN);    // read the value from the sensor

    frequency = FREQUENCY_MIN + ((val/10)*(FREQUENCY_MAX - FREQUENCY_MIN))/102;

    Serial.println(frequency);

    if (state == ON)
    {
        // Serial.println("state = ON");
        digitalWrite(RED_PIN, HIGH);
        digitalWrite(LED_STROBO_01, HIGH);
        //analogWrite(GREEN_PIN, BRIGHTNESS);
        //analogWrite(BLUE_PIN, BRIGHTNESS);

        // delay(500);
        delay((1000 / (frequency))*DUTY_CYCLE);

        state = OFF;
    }

    else
    {
        //Serial.println("state = OFF");
        digitalWrite(RED_PIN, LOW);
        digitalWrite(LED_STROBO_01, LOW);
        //analogWrite(GREEN_PIN, 0);
        //analogWrite(BLUE_PIN, 0);
        //delay(500);
        delay((1000 / (frequency))*(1-DUTY_CYCLE));

        state = ON;
    }
}

*/
