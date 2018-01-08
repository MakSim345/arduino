/* Simple test sketch for photoresitor

Connect photoresitor: one leg to 5 v, second - to pin "Analog 0"
Connect 10k resistor: one leg to ground, second - to pin "Analog 0" (common with photoresitor)
*/

//#define NANO_IN_USE
#define ARDUINO_IN_USE
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

#define CE_PIN   7 //4  // RST?
#define IO_PIN   6 //3  // DAT?
#define SCLK_PIN 5 //2  // CLK


#define DHTPIN 7     //  what pin we're connected to
#define VCCPIN 8     //
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN2 2


int POT_PIN = A0; // photoresitor and resistor 10к - connected to A0
int photocellReading; // read data from Analog pin
const int DIGIT_DELAY = 1000; // millisec
long tmp_update = 0; // 

void setup(void)
{
    // send debug info:
    Serial.begin(9600);
}

void loop(void)
{
    //displayNumber(_ctr);
    long time = millis();
    if (time >= tmp_update)
    {
        tmp_update = time + DIGIT_DELAY;
        
        photocellReading = analogRead(POT_PIN);
        Serial.print("Analog reading = ");
        Serial.print(photocellReading); // send analog values

        if (photocellReading < 10)
        {
            Serial.println(" - Dark");
        }
        else if (photocellReading < 200)
        {
            Serial.println(" - Dim");
        }
        else if (photocellReading < 500)
        {
            Serial.println(" - Light");
        }
        else if (photocellReading < 800)
        {
            Serial.println(" - Bright");
        }
        else
        {
            Serial.println(" - Very bright");
        }
    }
}

