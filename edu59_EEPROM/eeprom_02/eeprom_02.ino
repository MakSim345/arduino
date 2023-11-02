/*
OCT-2023
"http://codius.ru/articles"
*/

#include <EEPROM.h>

/*
    For empty EEPROM:
    CRC HEX - 7DCF4209
    CRC DEC - 2110734857

*/
const unsigned long freeEEPROM = 2110734857;

const unsigned long crc_table[16] = {
  // Fast CRC calculation table:
  0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
  0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
  0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
  0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};


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
    long actualEEPROM = eeprom_crc();

    for (int i = 0; i < EEPROM.length(); ++i)
    {
        // Reset EEPROM - all to ZERO.
        EEPROM.update(i, 255);
    }
    Serial.println("EEPROM fingerprint (CRC):");
    Serial.println(eeprom_crc(), HEX);
    Serial.println(eeprom_crc(), DEC);
}

void loop()
{
    static bool isEepromEmpty = true;
    int address = 0;
    int dataToWrite = 201;

    /*

    if (isEepromEmpty)
    {
        writeToEeprom(0, dataToWrite);
        isEepromEmpty = false;
    }

    readFromEeprom(address);

    delay(1000);
    */
}

unsigned long eeprom_crc(void)
{
  // Calculate CRC for EEPROM.
  unsigned long crc = ~0L;

  for (int index = 0 ; index < EEPROM.length()  ; ++index)
  {
    crc = crc_table[(crc ^ EEPROM[index]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (EEPROM[index] >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }
  return crc;
}
