/*
 An example of LED brightness
 on Arduino NANO, pin #XX
 using analogWrite().
 */
 #define PWM_LED_PIN_BLUE 5 // (D5)
 #define PWM_LED_PIN_RED  9 // (D9)
 #define PWM_LED_PIN_ONBOARD  13 // (D13)

int brightness_red = 0;    // LED brightness init
int brightness_blue = 255;    // LED brightness init
int fadeAmount = 1;    // step add/decrease brightness
int fade = -1;    // step add/decrease brightness
 
void setup()  
{
    //set pin PWM_LED_PIN to output mode:
    pinMode(PWM_LED_PIN_BLUE, OUTPUT);
    pinMode(PWM_LED_PIN_ONBOARD, OUTPUT);
    pinMode(PWM_LED_PIN_RED, OUTPUT); 
    Serial.begin(9600);   
    //pinMode(A2, OUTPUT); 
    //digitalWrite(A2, LOW); 
    //pinMode(A6, INPUT);
    //digitalWrite(A6, HIGH); //  // set pullup resistor on analog pin 6, while the pin is an INPUT.
}
 
void loop()  
{
    // set pwm value on the output pin
    // this will set LDE brightness.
    analogWrite(PWM_LED_PIN_BLUE, brightness_blue);   
    analogWrite(PWM_LED_PIN_RED,  brightness_red);   
    analogWrite(PWM_LED_PIN_ONBOARD, brightness_blue);
    // change brightness with predefined step:
    brightness_red = brightness_red + fadeAmount;
    brightness_blue = brightness_blue + fade;

    char out_buf[255];  //reserve the string space first
    // itoa(randOn, v_str, 6);
    // lcd.printIn(v_str);
    //int bytesSent = Serial.write(v_str); //send the string “hello” and return the length of the string.
    Serial.print("brightness: ");
    sprintf(out_buf, "%d : %d", brightness_red, brightness_blue);    
    Serial.println(out_buf);    // print the number
        
    // when max/min value of brightness had been approach
    // INVERT: change step's sign for going backward:
    if (brightness_red == 0 || brightness_red == 255) 
    {
        fadeAmount = -fadeAmount;
        fade = -fade;      
    }   
    // make a slight pause:
    delay(15);
}