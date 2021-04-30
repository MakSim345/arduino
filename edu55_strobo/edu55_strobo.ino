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
    // #define RED_PIN A0
    #define RED_PIN 11
    #define LED_STROBO_01 7
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

// long interval = 35;           // interval at which to blink (milliseconds)
long interval = 25;           // interval at which to blink (milliseconds)
// long strobInterval = 350;
long strobInterval = 500;
unsigned long currentMillis;

void setup()
{
    // set the digital pin as output:
    pinMode(RED_PIN, OUTPUT);
    pinMode(LED_STROBO_01, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    // here is where you'd put code that needs to be running all the time.

    // check to see if it's time to blink the LED; that is, if the
    // difference between the current time and last time you blinked
    // the LED is bigger than the interval at which you want to
    // blink the LED.
    unsigned long strobMillis = millis();

    if(strobMillis - previousStrobMillis > strobInterval)
    {
        strobState = !strobState;
        previousStrobMillis = strobMillis;
        Serial.println("state = OFF");
    }

    if (strobState)
    {
        currentMillis = millis();
        if (currentMillis - previousMillis > interval)
        {
            // save the last time you blinked the LED
            previousMillis = currentMillis;
            // if the LED is off turn it on and vice-versa:
            if (ledState == LOW)
                ledState = HIGH;
            else
                ledState = LOW;

            digitalWrite(RED_PIN, ledState);
            digitalWrite(LED_STROBO_01, ledState);
        }
    }
    else
    {
        digitalWrite(RED_PIN, HIGH);
        digitalWrite(LED_STROBO_01, HIGH);
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
