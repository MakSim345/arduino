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

#include <Wire.h>
#include <RTClib.h>
#include <Time.h>

// #define ledPin 7
#define ledPin 13 // embedded LED

// Connect pin 10 to BT->TX and 11 to BT->TX
SoftwareSerial bluetooth(10, 11);

RTC_DS3231 RTC;

void handleCommand(const String &cmdP);
String readStringFromBluetooth();

void setup ()
{
    Serial.begin(9600);
    // Wire.begin();
    RTC.begin();
    bluetooth.begin(9600);
    delay(1000);

    if(!RTC.begin())
    {
        Serial.println("No RTC Module found!");
        while(1);
    }

    // set internal LED (pin 13) as indicator:
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH); // set LED to ON for testing that it is working.

    Serial.println("Bluetooth test running...");

    bluetooth.println("Clock ready. Send: SET:YYYY-MM-DD HH:MM:SS");

    /*
    if (! RTC.isrunning())
    {
      Serial.println("RTC is NOT running!");
      // following line sets the RTC to the date & time this sketch was compiled
      RTC.adjust(DateTime(__DATE__, __TIME__));
    }
    */
    // RTC.adjust(DateTime(F(__DATE__),F( __TIME__)));
    // RTC.adjust(DateTime(2026, 6, 3, 9, 18, 0));
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

    RTC.adjust(DateTime(year, month, day, hour, minute, second));
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
    /*
    for (int i = 0; i < retMessage.length(); i++)
    {
        Serial.print(i);
        Serial.print("-Char: ");
        Serial.print(retMessage[i]);
        Serial.print("  ASCII: ");
        Serial.println((int)retMessage[i]);
    }
    */
    return retMessage;
}


void loop ()
{
    DateTime now = RTC.now();

    Serial.print(now.year(), DEC);
    Serial.print('-');
    if (now.month() < 10)
        Serial.print('0');
    Serial.print(now.month()), DEC;
    Serial.print('-');

    if (now.day() < 10)
        Serial.print('0');
    Serial.print(now.day(), DEC);
    Serial.print(' ');

    if (now.hour() < 10)
        Serial.print('0');
    Serial.print(now.hour());
    Serial.print(':');

    if (now.minute() < 10)
        Serial.print('0');
    Serial.print(now.minute());
    Serial.print(':');

    if (now.second() < 10)
        Serial.print('0');
    Serial.println(now.second());


     // wait for incoming data
    if (bluetooth.available())
    {
        String received_msg = readStringFromBluetooth();
        received_msg.trim();   // remove \r \n spaces

        Serial.println(received_msg);

        if (received_msg == "0")
        {
            // Turn LED OFF
            digitalWrite(ledPin, LOW);
            // Send back, to the phone, the String "LED: ON"
            bluetooth.write("LED: OFF\n");
        }
        else if (received_msg == "1")
        {
            digitalWrite(ledPin, HIGH);
            bluetooth.write("LED: ON\n");
        }
        else
        {
            // DEBUG:
            // bluetooth.write("Message unknown:\n");
            // bluetooth.println(received_msg);
            // Real handle:
            handleCommand(received_msg);
        }
    }

   /*int day_of_week = now.dayOfWeek();
   switch(day_of_week)
   {
     case 1:
      Serial.println("Monday");
      break;
     case 2:
      Serial.println("Tuesday");
      break;
     case 3:
      Serial.println("Wednesday");
      break;
     case 4:
      Serial.println("Thursday");
      break;
     case 5:
      Serial.println("Friday");
      break;
     case 6:
      Serial.println("Saturday");
      break;
     case 0:
      Serial.println("Sunday");
      break;
    }
    */
    delay(1000);
    /*
    time_t curTime = 0;
    // to Serial - unixtime
    if (Serial.available() > 0)
    {
      int eps = millis();
      time_t coef = 1000000000;
      while (coef > 0)
      {
         byte b = Serial.read();
         if (b == 255)
         {
            continue;
         }
         curTime += coef*(b-'0');
         coef /= 10;
      }
      // Set time RTC
      // RTC.adjust(curTime + (millis()-eps)/1000);
    }
    */
}

