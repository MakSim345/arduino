/*
Time measure project:
https://roboticsbackend.com/arduino-compute-duration-code-example/
*/

#define TEST_PIN 10

void setup()
{
    Serial.begin(9600);
    pinMode(TEST_PIN, OUTPUT);
}


void runRegisters()
{
    for(int i = 0; i < 500; i++)
    {
        // this one is fastest:
        //PORTB = B00000001; // Set pin to 1
        //PORTB = B00000000; // Set pin to 0

        // bit slower:
        //PORTB |=   1 << TEST_PIN;  // Set pin to 1
        // PORTB &= ~(1 << TEST_PIN); // Set pin to 0

        PORTB = PORTB | B00000001; // Set pin to 1
        PORTB = PORTB & B11111110; // Set pin to 0
    }
}

void runDigitalWrite()
{
    for (int i = 0; i < 700; ++i)
    {
        digitalWrite(TEST_PIN, HIGH);
        digitalWrite(TEST_PIN, LOW);
    }
}

void loop()
{
// ========================================
    unsigned long timeBegin = micros();
    runDigitalWrite();

    unsigned long timeEnd = micros();
    unsigned long timeDuration = timeEnd - timeBegin;
    double averageDuration = (double)timeDuration / 1000.0;
    Serial.print("DigitalWrite: ");
    Serial.println(averageDuration);
    delay(1000);

// ========================================
    timeBegin = micros();
    runRegisters();

    timeEnd = micros();
    timeDuration = timeEnd - timeBegin;
    averageDuration = (double)timeDuration / 1000.0;
    Serial.print("Registers in use: ");
    Serial.println(averageDuration);


}