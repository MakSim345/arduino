/*
 * Changing Module Settings via AT Commands
 * HC-05 and HC-06 modules accept AT commands for configuration including changing name,
 * PIN, baud rate, or other settings.
 *
 * Enter AT command mode by connecting module before powering on
 * (for HC-05, hold KEY pin HIGH during power-up).
 *
 * HC-06: Send commands like:
 * AT (test communication)
 * AT+NAME=MyRobot (change device name)
 * AT+PSWD=5678 (change PIN)
 * AT+UART=115200,0,0 (change baud rate)
**/

// Avoid using Arduino Serial Monitor while Bluetooth is connected to hardware TX/RX (use SoftwareSerial instead)
#include <SoftwareSerial.h>

// #define ledPin 7
#define ledPin 13

// use pin 10 as RX and 11 as TX
SoftwareSerial bluetooth(10, 11);

void setup()
{
    // Note: with app called "Serial Bluetooth Terminal" - only 9600 works!
    bluetooth.begin(9600);
    // bluetooth.begin(38400);

    Serial.begin(9600);

    // set internal LED (pin 13) as indicator:
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH); // set LED to ON for testing that it is working.

    Serial.println("Set up HC-05 to MASTER mode...");

    //Simple PING:
    Serial.println("Send 'AT'");
    bluetooth.println("AT");
    delay(1000);

    // Set bluetooth baudrate:
    //delay(1000);
    //bluetooth.println("AT+UART=9600,0,0");

    // Set as SLAVE:
    //Serial.println("Send 'AT+ROLE=0'");
    //bluetooth.println("AT+ROLE=0");
    //delay(1000);
    //bluetooth.println("AT+RESET");

    // Ask what is your role - MASTER or else?:
    Serial.println("Send 'AT+ROLE?'");
    bluetooth.println("AT+ROLE?");
    delay(1000);

    // Ask what is your PASS:
    Serial.println("Send 'AT+PSWD?'");
    bluetooth.println("AT+PSWD?");
    delay(1000);

    // Set as MASTER:
    //Serial.println("Send 'AT+ROLE=1'");
    //bluetooth.println("AT+ROLE=1");
    //delay(1000);
    //bluetooth.println("AT+RESET");
    //delay(1000);

    // Rename:
    //delay(1000);
    //Serial.println("Send 'AT+NAME=HC05_MASTER_01'");
    //bluetooth.println("AT+NAME=HC05_MASTER_01");

    //Serial.println("Send 'AT+NAME?'");
    //bluetooth.println("AT+NAME?");
    //delay(1000);

    // Set wide connection:
    //delay(1000);
    //Serial.println("Send 'AT+CMODE=1'");
    //bluetooth.println("AT+CMODE=1");
}

void loop()
{
    while (bluetooth.available())
    {
        Serial.write(bluetooth.read());
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
