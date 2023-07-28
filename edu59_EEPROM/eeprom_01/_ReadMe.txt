An example Arduino script that demonstrates how to write and read data from the EEPROM:

==============================================================
"https://alexgyver.ru/lessons/eeprom/"
==============================================================
#include <EEPROM.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Write data to EEPROM
  int address = 0;
  int data = 123;
  EEPROM.write(address, data);
  Serial.print("Wrote ");
  Serial.print(data);
  Serial.println(" to EEPROM");

  // Read data from EEPROM
  int readData = EEPROM.read(address);
  Serial.print("Read ");
  Serial.print(readData);
  Serial.println(" from EEPROM");

  delay(1000);
}

=============================================================
In this example, we start by including the EEPROM library. In the setup function, we initialize the serial communication at 9600 baud.

In the loop function, we first write the integer value 123 to EEPROM address 0 using the EEPROM.write function. We then print a message to the serial monitor indicating that we wrote data to EEPROM.

Next, we use the EEPROM.read function to read the data from EEPROM address 0 into the readData variable. We then print a message to the serial monitor indicating the value we read from EEPROM.

Finally, we add a delay of 1 second to give us time to read the output on the serial monitor before the loop starts over again.

==============================================================
#include <EEPROM.h>

void setup() 
{
  Serial.begin(9600);
  //EEPROM.begin(100);   // для esp8266/esp32
  // объявляем переменные, которые будем писать
  float dataF = 3.14;
  int16_t dataI = -634;
  byte dataArray[] = {10, 20, 30, 40};
  EEPROM.put(0, dataF);
  EEPROM.put(4, dataI);
  EEPROM.put(6, dataArray);
  //EEPROM.commit();     // для esp8266/esp32
  // объявляем переменные, куда будем читать
  float dataF_read = 0;
  int16_t dataI_read = 0;
  byte dataArray_read[4];
  // читаем точно так же, как писали
  EEPROM.get(0, dataF_read);
  EEPROM.get(4, dataI_read);
  EEPROM.get(6, dataArray_read);
  // проверяем
  Serial.println(dataF_read);
  Serial.println(dataI_read);
  Serial.println(dataArray_read[0]);
  Serial.println(dataArray_read[1]);
  Serial.println(dataArray_read[2]);
  Serial.println(dataArray_read[3]);
}
void loop() {}
==============================================================

==============================================================
