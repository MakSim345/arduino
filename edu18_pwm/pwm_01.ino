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
    delay(1);                           
}