#include <EEPROM.h>


void writeToEeprom(int addressP, int dataToWriteP)
{
    // write data to EEPROM
    // Note: address can be from 0 to 1023, i. e. 1kB
    EEPROM.write(addressP, dataToWriteP);
    Serial.print("Wrote ");
    Serial.print(dataToWriteP);
    Serial.println(" to EEPROM.");
}


void readFromEeprom(int addressP)
{
    // Read data from EEPROM by given address:
    int readData = EEPROM.read(addressP);
    Serial.print("Read ");
    Serial.print(readData);
    Serial.println(" from EEPROM");
    Serial.println("");
}

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    static bool isEepromEmpty = true;
    int address = 0;
    int dataToWrite = 201;

    if (isEepromEmpty)
    {
        writeToEeprom(0, dataToWrite);
        isEepromEmpty = false;
    }

    readFromEeprom(address);

    delay(1000);
}
