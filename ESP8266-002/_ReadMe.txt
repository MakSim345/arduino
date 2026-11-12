OCT-2026

The board is a "WeMos D1 R1".

It uses an "ESP8266" chip (ESP-12F module), not a usual "ESP32", and a CH340G chip for USB. The web page, the settings and the one-channel-after-another watering logic are the same as the ESP32 version.

What changed:

Libraries: the ESP32 ones (WiFi.h, ESPmDNS.h, WebServer.h) are replaced by the ESP8266 ones (ESP8266WiFi.h, ESP8266mDNS.h, ESP8266WebServer.h).

Pins: on this board the labels D0/D1 are the RX/TX serial pins, and names like D2 do not match the D1 mini or NodeMCU. So ESP8266-002.ino uses raw GPIO numbers, with a table in the header matching them to the board labels.

Pump: it now drives the blue LED on the ESP-12F module (GPIO2, labelled TX1/D9). That LED lights when the pin is LOW, so Machine.h inverts the signal. The pump also really dims the LED with PWM now, so the "Pump power (%)" setting changes the brightness. It sets the PWM range to 0–1023 because ESP8266 core 3.x defaults to 255.

mDNS: the ESP8266 needs MDNS.update() in loop(). The address is now http://esp8266.local.
Router (valve selector): still a dummy that only prints the channel.
It has GPIO 5, 16, 14, 12, the same chip pins as the original ESP8266 code, so it is ready if you wire real valves.

To upload in the Arduino IDE:

1. Add the ESP8266 boards package:
    - open Preferences, addidtional Boards Manager URLs
    - add "http://arduino.esp8266.com/stable/package_esp8266com_index.json",
    - open "Boards Manager".
    - install "esp8266" by ESP8266 community.
2. Select Tools > Board > ESP8266 Boards > LOLIN(WeMos) D1 R1.
3. Install the "CH340" USB driver if the COM port does not show up.
    Check: "C:\Dropbox\07_Utils\Programmer Utils\microcontrollers\CH341SER"
4. Put your SSID and password in the sketch and set the Serial Monitor to 115200 baud.

Note: Do not put more than 3.3 V on any pin, including A0, even though the headers are Uno-style. The ESP8266 is not 5 V tolerant.



