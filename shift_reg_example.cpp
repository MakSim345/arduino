/*
 *
 * 74HC164 test
 *
 * outputs 3, 4, 5, 6, 10, 11, 12, 13 to LEDs
 * serial A (1) - pull up to VCC
 * serial B (2) - srDATA
 * gnd 7 - GND
 * clock 8 - srCLOCK
 * clear 9 - srCLEAR
 * vcc 14 - VCC
 */

#define srCLOCK 10
#define srCLEAR 8
#define srDATA 6

void setup()
{
  pinMode(srCLOCK, OUTPUT);
  pinMode(srCLEAR, OUTPUT);
  pinMode(srDATA, OUTPUT);
  digitalWrite(srCLEAR, HIGH);
}

void loop()
{
  // binary representation of 0 - 255
  for (int i = 0; i <= 255; i++) {
    shiftOut(srDATA, srCLOCK, LSBFIRST, i);
    delay(150);
  }

  // pretty pointless scrolling lights
  for (int i = 0; i < 500; i++) {
    for (int j = 0; j < 8; j++) {
      shiftOut(srDATA, srCLOCK, MSBFIRST, 1 << j);
      delay(50);
    }
  }
}