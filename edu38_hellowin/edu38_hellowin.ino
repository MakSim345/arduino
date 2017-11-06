/* 
SEP-2014.
An app for Hellowin pumpkins lights: red, blue and white
 */

#define LED_PIN      7
#define PWM_LED_PIN  9
#define PWM_LED_PIN2 6

// const unsigned int LED_PIN = A0;
// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
long prev_fade_millis = 0;      // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 80;           // interval at which to blink (milliseconds)
long fade_interval = 4;      // interval at which to fade
long fire_interval = 100;      // interval at which to fade

int brightness = 0;    // LED brightness init
int fadeAmount = 1;    // step add/decrease brightness

long randOn = 0;       


void setup() 
{
    // set the digital pin as output:
    pinMode(LED_PIN, OUTPUT);
    pinMode(PWM_LED_PIN, OUTPUT);
    pinMode(PWM_LED_PIN2, OUTPUT);
    randomSeed (analogRead (0));    // randomize 
    // interval = random (100, 1200);    // generate ON time between 0.1 and 1.2 seconds
}

void fadeLed()
{
    // set pwm value on the output pin
    // this will set LDE brightness.
    analogWrite(PWM_LED_PIN, brightness);   
 
    // change brightness with predefined step:
    brightness = brightness + fadeAmount;
 
    // when max/min value of brightness had been approach
    // change step's sign for going backward:
    if (brightness == 0 || brightness == 255) 
    {
        fadeAmount = -fadeAmount;
    }
}

void fire_simul()
{
    //analogWrite(ledPin1, random(120)+135);
    //analogWrite(ledPin2, random(120)+135);
    // analogWrite(ledPin3, random(120)+135);
    analogWrite(PWM_LED_PIN, random(120)+135);   
    analogWrite(PWM_LED_PIN2, random(120)+135);   
    //delay(random(100));
}

void loop()
{
    // here is where you'd put code that needs to be running all the time.

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
        digitalWrite(LED_PIN, ledState);
        // interval = random (100, 1200);    // generate ON time between 0.1 and 1.2 seconds
    }

    if(currentMillis - prev_fade_millis > fire_interval) 
    {
        // save the last time you blinked the LED
        prev_fade_millis = currentMillis;
        // fadeLed();
        fire_simul();
        fire_interval = random (100);
    }
}