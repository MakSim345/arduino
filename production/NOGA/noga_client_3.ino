void ReceiveCommands2()
{
    bool ledStatus = false;
    char receivedData[64];
    int dataIndex = 0;

    if (Serial.available())
    {
        // byteRead = Serial.read();
        char recievedChar = Serial.read();

        // Process message when new line character is recieved
        if (recievedChar == '\n')
        {
            receivedData [dataIndex] = '\0'; // null-terminate
            // Send back received message with "OK" appended
            Serial.print(receivedData);
            Serial.println(" OK.");
            
            // Perform actions based on specific commands
            if (receivedData == "status")
            {
                Serial.println( (ledStatus) ? "LED: ON" : "LED: OFF");
            }

            if (receivedData == "ON")
            {
                digitalWrite(LED_BUILTIN, HIGH);  // Turn on the onboard LED
                ledStatus = true;
            }

            else if (receivedData == "OFF")
            {
                digitalWrite(LED_BUILTIN, LOW);   // Turn off the onboard LED
                ledStatus = false;
            }

            receivedData[dataIndex] = recievedChar;
            dataIndex++;
        }
    }
}
