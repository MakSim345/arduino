An example Arduino script that demonstrates how to write and read data from the EEPROM:

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
