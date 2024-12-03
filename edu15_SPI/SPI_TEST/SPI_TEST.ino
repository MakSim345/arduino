#include <SPI.h>

/*
On the Arduino Nano, the SPI pins are as follows:
- MOSI (Master Out Slave In): Pin D11
- MISO (Master In Slave Out): Pin D12
- SCK (Serial Clock): Pin D13
- SS (Slave Select): Typically, you can use D10, but you can choose any digital pin for SS when you configure it in your code.
*/

const int slaveSelectPin = 10; // Pin for Slave Select (SS)

// Setup function
void setup() {
    // Initialize Serial for debugging
    Serial.begin(9600);

    // Set the Slave Select pin as OUTPUT
    pinMode(slaveSelectPin, OUTPUT);
    digitalWrite(slaveSelectPin, HIGH); // Ensure SS is high

    // Initialize SPI
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV8); // Set SPI clock speed (8 MHz with 16 MHz Arduino Nano)
}

// Loop function
void loop() {
    byte dataToSend = 0x42; // Example data to send
    byte receivedData;

    // Start communication with the slave
    digitalWrite(slaveSelectPin, LOW); // Enable the slave

    // Send data
    // receivedData = SPI.transfer(dataToSend); // Send and receive data
    delay(10);
    // End communication
    digitalWrite(slaveSelectPin, HIGH); // Disable the slave

    // Print the sent and received data
    Serial.print("Sent: 0x");
    Serial.print(dataToSend, HEX);
    Serial.print(" | Received: 0x");
    Serial.println(receivedData, HEX);

    // Wait for a second before the next loop iteration
    delay(1000);
}
