/*
 * Changing Module Settings via AT Commands
 * HC-05 and HC-06 modules accept AT commands for configuration including changing name,
 * PIN, baud rate, or other settings.
 *
 * Enter AT command mode by connecting module before powering on
 * (for HC-05, hold KEY pin HIGH during power-up).
 *
 * Send commands like:
 * AT (test communication)
 * AT+NAME=MyRobot (change device name)
 * AT+PSWD=5678 (change PIN)
 * AT+UART=115200,0,0 (change baud rate)
**/

// Avoid using Arduino Serial Monitor while Bluetooth is connected to hardware TX/RX (use SoftwareSerial instead)
#include <SoftwareSerial.h>

// #define ledPin 7
#define ledPin 13

// use pin 7 as RX and 8 as  TX
SoftwareSerial bluetooth(7, 8);

void setup()
{
    // Note: with app called "Serial Bluetooth Terminal" - only 9600 works!
    bluetooth.begin(9600);

    Serial.begin(9600);

    // set internal LED (pin 13) as indicator:
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH); // set LED to ON for testing that it is working.

    Serial.println("Bluetooth test running...");

    bluetooth.println("Clock ready. Send: SET:YYYY-MM-DD HH:MM:SS");
}

void loop()
{
    // wait for incoming data
    if (bluetooth.available())
    {
        String received_msg = readStringFromBluetooth();
        received_msg.trim();   // remove \r \n spaces

        Serial.print(received_msg);

        if (received_msg == "0")
        {
            digitalWrite(ledPin, LOW); // Turn LED OFF
            bluetooth.write("LED: OFF\n"); // Send back, to the phone, the String "LED: ON"
        }
        else if (received_msg == "1")
        {
            digitalWrite(ledPin, HIGH);
            bluetooth.write("LED: ON\n");
        }
        else
        {
            // bluetooth.write("Message unknown:\n");
            // bluetooth.println(received_msg);
            handleCommand(received_msg);
        }
    }

    // wait for the user to type a message into the serial monitor and
    // send it to the connected device
    if (Serial.available())
    {
        bluetooth.write(Serial.read());
    }
}

void handleCommand(const String &cmdP)
{
    if (!cmdP.startsWith("SET:"))
    {
        bluetooth.println("ERR: Unknown command");
        return;
    }

    if (cmdP.length() != 23)
    {
        bluetooth.println("ERR: Incorrect command lenght");
        bluetooth.println(cmdP.length());
        return;
    }

    int year   = cmdP.substring(4, 8).toInt();
    int month  = cmdP.substring(9, 11).toInt();
    int day    = cmdP.substring(12, 14).toInt();
    int hour   = cmdP.substring(15, 17).toInt();
    int minute = cmdP.substring(18, 20).toInt();
    int second = cmdP.substring(21, 23).toInt();

    if (year < 2020 || month < 1 || month > 12 || day < 1 || day > 31 ||
      hour > 23 || minute > 59 || second > 59)
    {
        bluetooth.println("ERR: invalid date/time");
        return;
    }

     bluetooth.println("OK: new date/time set.");
}

String readStringFromBluetooth()
{
    String retMessage = "";

    while(bluetooth.available())
    {
        // read one char from bluetooth connection and add to the message
        // char inChar = bluetooth.read();
        // retMessage = retMessage + inChar;
        retMessage = bluetooth.readStringUntil('\n');
    }

    //DEBUG:

    for (int i = 0; i < retMessage.length(); i++)
    {
        Serial.print(i);
        Serial.print("-Char: ");
        Serial.print(retMessage[i]);
        Serial.print("  ASCII: ");
        Serial.println((int)retMessage[i]);
    }

    return retMessage;
}

// #define SIMPLE_SERIAL_BT
#ifdef SIMPLE_SERIAL_BT
int state = 0;

void setup()
{
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
    // Note: with app called "Serial Bluetooth Terminal" - only 9600 works!
    Serial.begin(9600); // OK!
    // Serial.begin(38400); //NOT working! (Default rate of the Bluetooth module)
    // Serial.begin(115200); //NOT working!
}

void loop()
{
    if(Serial.available() > 0)
    {
        // Checks whether data is comming from the serial port
        state = Serial.read(); // Reads the data from the serial port
    }

    if (state == '0')
    {
        digitalWrite(ledPin, LOW); // Turn LED OFF
        Serial.println("LED: OFF"); // Send back, to the phone, the String "LED: ON"
        state = 0;
    }
    else if (state == '1')
    {
        digitalWrite(ledPin, HIGH);
        Serial.println("LED: ON");;
        state = 0;
    }
}
#endif // SIMPLE_SERIAL_BT
