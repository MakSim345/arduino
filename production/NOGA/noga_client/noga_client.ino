#include <Servo.h>

#define SERVO_PIN 9
#define MIN_SECONDS_DELAY 2000
#define MAX_SECONDS_DELAY 9000
#define SET_TARGET_DELAY 1000

#ifdef ARDUINO_IN_USE
    #define DATA_IN_PIN 12
    #define CLK_PIN 11
    #define LOAD_PIN 10
#endif
/*
 Arduino NANO, pin #XX
 */
#ifdef NANO_IN_USE
    #define DATA_IN_PIN 5  // (D5)
    #define CLK_PIN     12 // (D9)
    #define LOAD_PIN    13 // (D13)
#endif

const int startPosition = 160;  // Start position of the servo
const int endPosition = 950;  // End position of the servo
int currentPos = startPosition;  // Current position of the servo


unsigned long startDelay = 1200;  // Delay at the start position (in milliseconds)
unsigned long endDelay = 1200;    // Delay at the end position (in milliseconds)
unsigned long delayTime = startDelay;  // Initial delay time

// ATTN: a maximum of eight servo objects can be created
Servo noga_servo;

int targetPosition = 0;  // Initial position of the servo
unsigned long previousTime = 0;  // Variable to store the previous time


int convertDelaysToRespRate(unsigned long currentDelayMsP)
{
    // Get the Respiration Rate and 
    int currentRespRate = 60.0 / ((currentDelayMsP*2)/1000.0);
    return currentRespRate;
}

void setDelays(unsigned long newRespRateP)
{
    // Get the Respiration Rate and 
    // calculate delay in ms for I:E (in 50/50 way)
    int newDelay = 60000/(2.0 * newRespRateP);
    
    startDelay = newDelay;
    endDelay = startDelay;

    delayTime = startDelay;
}

void setup()
{
    Serial.begin(9600);
    noga_servo.attach(SERVO_PIN, 500, 2500); //the servo on pin 9 to the servo object
    // noga_servo.writeMicroseconds(1500);  // set servo to mid-point
    previousTime = millis();  // Set the initial previous time
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    static bool ledStatus = false;
    
    unsigned long currentTime = millis();  // Get the current time

    // Check if the time interval has passed
    if (currentTime - previousTime >= delayTime)
    {
        previousTime = currentTime;  // Update the previous time

        // Move the servo to the target position
        // noga_servo.write(currentPos);
        noga_servo.writeMicroseconds(currentPos);

        if (currentPos == startPosition)
        {
            currentPos = endPosition;
            delayTime = endDelay;
        }
        else
        {
            currentPos = startPosition;
            delayTime = startDelay;
        }
    }

/*
    // noga_servo.write(0);
    noga_servo.writeMicroseconds(startPosition);
    delay(startDelay);

    // noga_servo.write(45);
    noga_servo.writeMicroseconds(endPosition);
    delay(endDelay);

    // move the servo to 180 degrees
    //noga_servo.write(180);
    //delay(1000); // wait for 1 second
*/
    if (Serial.available())
    {
        String receivedData = Serial.readStringUntil('\n');
        receivedData.trim();

        // Perform actions based on specific commands
        if (receivedData.startsWith("status"))
        {
            Serial.print( (ledStatus) ? "LED: ON" : "LED: OFF");
        }

        else if (receivedData.startsWith("ON"))
        {
            digitalWrite(LED_BUILTIN, HIGH);  // Turn on the onboard LED
            ledStatus = true;
        }

        else if (receivedData.startsWith("OFF"))
        {
            digitalWrite(LED_BUILTIN, LOW);   // Turn off the onboard LED
            ledStatus = false;
        }

        else if (receivedData.startsWith("GET_RR"))
        {
            // getBreakpointValue();
            Serial.print("RR is ");
            int CurrentRR = convertDelaysToRespRate(startDelay);            
            Serial.print(CurrentRR);
        }
        else if (receivedData.startsWith("SET_RR"))
        {
            int spaceIndex = receivedData.indexOf(' ');
            if (spaceIndex != -1)
            {
                String valueString = receivedData.substring(spaceIndex + 1);
                int TargetRR = valueString.toInt();
                Serial.print("RR set to: ");
                Serial.print(TargetRR);
                setDelays(TargetRR);
            }
            else
            {
                Serial.print("ERROR: Invalid SET_RR command format!");
            }
        }
        else if (receivedData.startsWith("OFF"))
        {
            digitalWrite(LED_BUILTIN, LOW);   // Turn off the onboard LED
            ledStatus = false;
        }
        else
        {
            // Unknown command
            Serial.print("ERROR: Unknown command!");
        }
        // Send back received message with "OK" appended
        // Serial.println(receivedData + " OK");
    }


}
