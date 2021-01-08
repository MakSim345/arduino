#include "virtuabotixRTC.h"

#define CE_PIN   4  // RST?
#define IO_PIN   3  // DAT?
#define SCLK_PIN 2  // CLK

// Creation of the Real Time Clock Object
// virtuabotixRTC(uint8_t inSCLK, uint8_t inIO, uint8_t inC_E);
virtuabotixRTC myRTC(SCLK_PIN, IO_PIN, CE_PIN);

void setup()
{
  Serial.begin(9600);

  // Set the current date, and time in the following format:
  // seconds, minutes, hours, day of the week, day of the month, month, year
  //                  SS  MM  HH DW DD  MM  YYYY
  // myRTC.setDS1302Time(00, 21, 9, 2, 13, 10, 2015);
}

void loop()
{
// This allows for the update of variables for time or accessing the individual elements.
  myRTC.updateTime();

  // Start printing elements as individuals
  Serial.print("Current Date / Time: ");
  Serial.print(myRTC.dayofmonth);
  Serial.print("/");
  Serial.print(myRTC.month);
  Serial.print("/");
  Serial.print(myRTC.year);
  Serial.print("  ");
  Serial.print(myRTC.hours);
  Serial.print(":");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  Serial.println(myRTC.seconds);

  // Delay
  delay(2000);
}

