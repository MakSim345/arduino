#include <EEPROM.h>

int address = 0;

// user defined structure
struct myStruct
{
    float sum;
    byte age;
    char fullname[15];
};

struct MySettings
{
    float calibration_val;
    int modeID;
    char deviceName[10];
};

void setup()
{
    Serial.begin(9600);

    // WRITE
    int ledBrightness = 155;
    // Use update to save cell 0 resoiurce
    // EEPROM.update(0, ledBrightness);

    MySettings customConfig = {3.14, 2, "Sensor_A"};
    // Write the whole structure starting from address 10
    // EEPROM.put(10, customConfig);

}

void loop()
{
    // Better to keep empty to keep cell resources
    // READ:
    int readValue = EEPROM.read(0);

    MySettings savedConfig;
    EEPROM.get(10, savedConfig);

    // Serial printing:
    Serial.print("EEPROM: LED Brightness: ");
    Serial.println(readValue);

    Serial.print("EEPROM: Saved Config: modeID = ");
    Serial.println(savedConfig.modeID);

    Serial.print("EEPROM: Saved Config: deviceName = ");
    Serial.println(savedConfig.deviceName);

    Serial.print("EEPROM: Saved Config: calibration_val = ");
    Serial.println(savedConfig.calibration_val);
    delay(10000);
}

/*
void setup()
{
    Serial.begin(9600);

    myStruct person = {};

    EEPROM.get(address, person);
    Serial.println("Read user defined structure from EEPROM");
    Serial.println(person.sum);
    Serial.println(person.age);
    Serial.println(person.fullname);
}

void loop() {}
*/
