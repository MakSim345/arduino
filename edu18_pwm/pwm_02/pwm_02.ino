/*
 An example of LED brightness
 on Arduino MCU, pin #XX
 using analogWrite().
 */
#define PWM_LED_PIN 9
int brightness = 0;    // LED brightness init
int fadeAmount = 1;    // step add/decrease brightness
 
void setup()  
{
    //set pin PWM_LED_PIN to output mode:
    pinMode(PWM_LED_PIN, OUTPUT);
}
 
void loop()  
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
        fadeAmount = -fadeAmount ;
    }   
    // make a slight pause:
    delay(5);                           
}


// #define PWM_TEST
#ifdef PWM_TEST
/*
 An example of LED brightness
 on Arduino MCU, pin #XX
 using analogWrite().
 */
 #define PWM_LED_PIN  3
 #define PWM_LED_PIN_11 3 // ORIG - 3
 #define PWM_LED_PIN_09 2
 #define LED_PIN 13

int brightness = 0;    // LED brightness init
int fadeAmount = 1;    // step add/decrease brightness
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated

// the follow variable is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 3000;           // interval at which to blink (milliseconds)

int main_Pin = PWM_LED_PIN_11;
int gnd_Pin = PWM_LED_PIN_09;
int flag_11 = 1;
bool isOnce = true;

void setMotorDirection(int _motor_direct)
{
    if (0 == _motor_direct)
    {
        digitalWrite(main_Pin, HIGH);   
        digitalWrite(gnd_Pin, LOW);   
    }
    else
    {
        digitalWrite(main_Pin, LOW);  
        digitalWrite(gnd_Pin, HIGH);  
    }        
}
 
void setup()  
{
    //set pin PWM_LED_PIN to output mode:
    pinMode(PWM_LED_PIN, OUTPUT);
    pinMode(PWM_LED_PIN_11, OUTPUT);    
    pinMode(PWM_LED_PIN_09, OUTPUT);
}
 
void loop()  
{
    unsigned long currentMillis = millis();    
    // analogWrite(gnd_Pin, 0); 
    // set pwm value on the output pin
    // this will set LDE brightness.
    // analogWrite(PWM_LED_PIN, brightness);   
    // analogWrite(main_Pin, brightness);   
 
    if (0) // (isOnce == true)
    {
        digitalWrite(main_Pin, HIGH);   // sets the LED on
        digitalWrite(gnd_Pin, LOW);   // sets the LED on

        delay(1000);

        digitalWrite(main_Pin, LOW);   // sets the LED on
        digitalWrite(gnd_Pin, HIGH);   // sets the LED on
        delay(1000);
        
        isOnce = false;
    }
    // change brightness with predefined step:
    brightness = brightness + fadeAmount;
 
    // when max/min value of brightness had been approach
    // change step's sign for going backward:
    if (brightness == 0 || brightness == 255) 
    {
        fadeAmount = -fadeAmount ;
    }   
    // make a slight pause:
    // delay(25);
    //digitalWrite(main_Pin, HIGH);   // sets the LED on
    //digitalWrite(gnd_Pin, LOW);   // sets the LED on
    //setMotorDirection(0);
    //delay(5000);
    
    //digitalWrite(main_Pin, LOW);   // sets the LED on
    //digitalWrite(gnd_Pin, HIGH);   // sets the LED on
    //setMotorDirection(1);
    //delay(5000);

    if(currentMillis - previousMillis > interval) 
    {
        // save the last time you blinked the LED
        previousMillis = currentMillis;

        //digitalWrite(main_Pin, LOW);   // sets the LED on
        //digitalWrite(gnd_Pin, LOW);   // sets the LED on

        // if the LED is off turn it on and vice-versa:
        if (ledState == LOW)
        {
            ledState = HIGH;
            setMotorDirection(0);
        }            
        else
        {
            ledState = LOW;
            setMotorDirection(1);
        }
        // set the LED with the ledState of the variable:
        digitalWrite(LED_PIN, ledState);
    }
}
#endif
