/*
 An example of LED brightness
 on Arduino NANO, pin #XX
 using analogWrite().
 */
 #define PWM_LED_PIN_BLUE 5 // (D5)
 #define PWM_LED_PIN_RED  9 // (D9)
 #define PWM_LED_PIN_ONBOARD  13 // (D13)
int brightness = 0;    // LED brightness init
int fadeAmount = 1;    // step add/decrease brightness
 
void setup()  
{
    //set pin PWM_LED_PIN to output mode:
    pinMode(PWM_LED_PIN_BLUE, OUTPUT);
    pinMode(PWM_LED_PIN_RED, OUTPUT); 
}
 
void loop()  
{
    // set pwm value on the output pin
    // this will set LDE brightness.
    analogWrite(PWM_LED_PIN_BLUE, brightness);   
    analogWrite(PWM_LED_PIN_RED, brightness);   
 
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