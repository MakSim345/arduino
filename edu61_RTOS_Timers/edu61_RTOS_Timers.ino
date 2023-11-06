/*
OCT-2023
Multitasking on Arduino
https://habr.com/ru/articles/413779/
*/


// const unsigned int LED_PIN = 7;
const unsigned int LED_PIN = 13;

int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated

long interval = 1000;           // interval at which to blink (milliseconds)
unsigned long counter = 0;

bool cycle_1s = false;
bool cycle_5s = false;
bool cycle_10s = false;
bool cycle_20s = false;
bool cycle_30s = false;
bool cycle_1m = false;
bool cycle_2m = false;
bool cycle_3m = false;
bool cycle_5m = false;
bool cycle_10m = false;


unsigned long timeSec;
unsigned long timer_1s;
unsigned long timer_5s;
unsigned long timer_10s;
unsigned long timer_20s;
unsigned long timer_30s;
unsigned long timer_1m;
unsigned long timer_2m;
unsigned long timer_3m;
unsigned long timer_5m;
unsigned long timer_10m;


void timersInit()
{
    unsigned long uptimeSec = millis() / 1000;
    timeSec = uptimeSec;
    timer_1s = uptimeSec;
    timer_5s = uptimeSec;
    timer_10s = uptimeSec;
    timer_20s = uptimeSec;
    timer_30s = uptimeSec;
    timer_1m = uptimeSec;
    timer_2m = uptimeSec;
    timer_3m = uptimeSec;
    timer_5m = uptimeSec;
    timer_10m = uptimeSec;
}

void eraseCycles()
{
    cycle_1s = false;
    cycle_5s = false;
    cycle_10s = false;
    cycle_20s = false;
    cycle_30s = false;
    cycle_1m = false;
    cycle_2m = false;
    cycle_3m = false;
    cycle_5m = false;
    cycle_10m = false;
}

void timersWork()
{
    timeSec = millis() / 1000;

    if (timeSec - timer_1s >= 1)
    {
        timer_1s = timeSec;
        cycle_1s = true;
    }
    if (timeSec - timer_5s >= 5)
    {
        timer_5s = timeSec;
        cycle_5s = true;
    }
    if (timeSec - timer_10s >= 10)
    {
        timer_10s = timeSec;
        cycle_10s = true;
    }
    if (timeSec - timer_20s >= 20)
    {
        timer_20s = timeSec;
        cycle_20s = true;
    }
    if (timeSec - timer_30s >= 30)
    {
        timer_30s = timeSec;
        cycle_30s = true;
    }
    if (timeSec - timer_1m >= 60)
    {
        timer_1m = timeSec;
        cycle_1m = true;
    }
    if (timeSec - timer_2m >= 120)
    {
        timer_2m = timeSec;
        cycle_2m = true;
    }
    if (timeSec - timer_3m >= 180)
    {
        timer_3m = timeSec;
        cycle_3m = true;
    }
    if (timeSec - timer_5m >= 300)
    {
        timer_5m = timeSec;
        cycle_5m = true;
    }
    if (timeSec - timer_10m >= 600)
    {
        timer_10m = timeSec;
        cycle_10m = true;
    }
}


void PrintBoolValueToSerial(const bool &toPrintP)
{
    if (toPrintP)
        Serial.println("True");
    else
        Serial.println("False");
}

void PrintAllVariablesToSerial(const int &counterP)
{
    Serial.println(counterP);

    Serial.print("cycle_1s - ");
    PrintBoolValueToSerial(cycle_1s);

    Serial.print("cycle_5s - ");
    PrintBoolValueToSerial(cycle_5s);

    Serial.print("cycle_10s - ");
    PrintBoolValueToSerial(cycle_10s);

    Serial.print("cycle_20s - ");
    PrintBoolValueToSerial(cycle_20s);

    Serial.print("cycle_30s - ");
    PrintBoolValueToSerial(cycle_30s);

    Serial.print("cycle_1m - ");
    PrintBoolValueToSerial(cycle_1m);

    Serial.print("cycle_2m - ");
    PrintBoolValueToSerial(cycle_2m);

    Serial.print("cycle_3m - ");
    PrintBoolValueToSerial(cycle_3m);

    Serial.print("cycle_5m - ");
    PrintBoolValueToSerial(cycle_5m);

    Serial.print("cycle_10m - ");
    PrintBoolValueToSerial(cycle_10m);

    Serial.println("---------------------------------------");
}

void setup()
{
    Serial.begin(9600);
    // set the digital pin as output:
    pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    // unsigned long currentMillis = millis();
    timersWork();
    if (cycle_1s)
    {
        // if the LED is off turn it on and vice-versa:
        if (ledState == LOW)
            ledState = HIGH;
        else
            ledState = LOW;
        // set the LED with the ledState of the variable:
        digitalWrite(LED_PIN, ledState);
        counter++;
        PrintAllVariablesToSerial(counter);
    }

/*
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
    }
*/
    eraseCycles();
}

