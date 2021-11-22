/*
2014-SEP.
An app for Hellowin pumpkins lights: red, blue and white

2017-OCT.
Update sketch to use with NANO
*/

// #define NANO_IN_USE
#define UNO_IN_USE

// same on UNO and NANO:
const unsigned int TST_LED_PIN = 13; // not PWM!

#ifdef UNO_IN_USE
    const unsigned int PWM_LED_PIN_RED = 9; //in this pin PWM pin in use!
    const unsigned int PWM_LED_PIN_BLU = 6; //in this pin PWM pin in use!
    // const unsigned int PWM_LED_PIN_BLU = TST_LED_PIN; // 13 is not PWM
#endif


#ifdef NANO_IN_USE
    const unsigned int PWM_LED_PIN_RED = 9; //PWM pin in use!
    const unsigned int PWM_LED_PIN_BLU = 6; //PWM pin in use!
#endif


// const unsigned int LED_PIN = A0;
// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
long prev_fire_millis = 0;      // will store last time LED was updated
long prev_fade_millis = 0;      // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 500;           // interval at which to blink (milliseconds)
long fade_interval = 4;      // interval at which to fade
long fire_interval = 100;      // interval at which to fade

int brightness = 0;    // LED brightness init
int fadeAmount = 1;    // step add/decrease brightness

long randOn = 0;

void setup()
{
    // set the digital pin as output:
    pinMode(TST_LED_PIN, OUTPUT);
    pinMode(PWM_LED_PIN_RED, OUTPUT);
    pinMode(PWM_LED_PIN_BLU, OUTPUT);
    randomSeed (analogRead (0));    // randomize
    // interval = random (100, 1200);    // generate ON time between 0.1 and 1.2 seconds
}

void fadeLed()
{
    // set pwm value on the output pin
    // this will set LDE brightness.
    analogWrite(PWM_LED_PIN_RED, brightness);

    // change brightness with predefined step:
    brightness = brightness + fadeAmount;

    // when max/min value of brightness had been approach
    // change step's sign for going backward:
    if (brightness == 0 || brightness == 255)
    {
        fadeAmount = -fadeAmount;
    }
}

void fire_simulation()
{
    //analogWrite(ledPin1, random(120)+135);
    //analogWrite(ledPin2, random(120)+135);
    // analogWrite(ledPin3, random(120)+135);

    // analogWrite(PWM_LED_PIN_RED, random(120)+135);
    analogWrite(PWM_LED_PIN_BLU, random(120)+130);
    //delay(random(100));
}

void loop()
{
    // check to see if it's time to blink the LED; that is, if the
    // difference between the current time and last time you blinked
    // the LED is bigger than the interval at which you want to
    // blink the LED.
    unsigned long currentMillis = millis();

    if(currentMillis - previousMillis > interval)
    {
        // save the last time you blinked the LED
        previousMillis = currentMillis;

        // if the LED is off turn it on and vice-versa:
        if (ledState == LOW)
            ledState = HIGH;
        else
            ledState = LOW;

        // set the LED with the ledState of the variable:
        digitalWrite(TST_LED_PIN, ledState);
        // interval = random (10, 1200);    // generate ON time between 0.1 and 1.2 seconds
    }

    if(currentMillis - prev_fire_millis > fire_interval)
    {
        // save the last time you blinked the LED
        prev_fire_millis = currentMillis;
        // fadeLed();
        fire_simulation();
        fire_interval = random (100, 1200);
    }

    if(currentMillis - prev_fade_millis > fade_interval)
    {
        // save the last time you blinked the LED
        prev_fade_millis = currentMillis;
        fadeLed();
    }
}
