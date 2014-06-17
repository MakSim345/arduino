// Read analog value and send it using serial
//
// Copyright (C) 2012 Asaf Paris Mandoki http://asaf.pm
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//   This program for Arduino reads one channel and sends the data
//   out through the serial port in 2 bytes.
//   For synchronization purposes, the following scheme was chosen:
//   A0 data:   A09 (MSB) A08 A07 A06 A05 A04 A03 A02 A01 A00 (LSB)
//   sent as byte 1:   1 1 1 A09 A08 A07 A06 A05
//       and byte 2:   0 1 1 A04 A03 A02 A01 A00
//
//
//
int sensorValue = 0;        // value read from the pot
byte lb;
byte hb;
int LED_PIN = 13;
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.

long interval = 1000;           // interval at which to blink (milliseconds)

int get_sin_val();

void setup() 
{
  // initialize serial communications at 115200 bps:
  Serial.begin(115200); 

  // randomize:
  randomSeed (analogRead (0));
  pinMode(LED_PIN, OUTPUT); 
}

void loop() 
{
  unsigned long currentMillis = millis();
/*
    if(currentMillis - previousMillis > interval) 
    {
        // save the last time you blinked the LED
        previousMillis = currentMillis;

        // if the LED is off turn it on and vice-versa:
        if (ledState == LOW)
            ledState = HIGH;
        else
            ledState = LOW;

        // set the LED with the ledState of the variable:
        digitalWrite(LED_PIN, ledState);
    }
 */
  // read A0:
  sensorValue = analogRead(A0);

  Serial.println(sensorValue);             // debug value
  /*
  // sensorValue = random (2, 300);    // generate random value between 0.2 and 0.9 seconds
  // sensorValue = get_sin_val();
  
  // shift sample by 3 bits, and select higher byte  
  hb=highByte(sensorValue<<3); 
  // set 3 most significant bits and send out
  Serial.write(hb|0b11100000); 
  // select lower byte and clear 3 most significant bits
  lb=(lowByte(sensorValue))&0b00011111;
  // set bits 5 and 6 and send out
  Serial.write(lb|0b01100000);  
*/
}


int get_sin_val()
{
  static int i = 0;
  static bool isGoUp = true;
  int ret_val = 0;

  if (isGoUp)
  {
    i = i + 1;
    if (i > 500)
    {
      isGoUp = false;
      ret_val = 50;
    }      
  }
  else
  {
    i = i - 1;
    if (i < 0)
    {
      isGoUp = true;
      ret_val = 100;
    }
  }
  return i;
}  