#include "virtuabotixRTC.h"

// #define NANO_IN_USE
#ifdef NANO_IN_USE
    #define CE_PIN   4  // RST?
    #define IO_PIN   3  // DAT?
    #define SCLK_PIN 2  // CLK
#endif

#define UNO_IN_USE
#ifdef UNO_IN_USE
    #define CE_PIN   13  // RST?
    #define IO_PIN   12  // DAT?
    #define SCLK_PIN 11  // CLK
#endif

// Creation of the Real Time Clock Object
// virtuabotixRTC(uint8_t inSCLK, uint8_t inIO, uint8_t inC_E);
virtuabotixRTC myRTC(SCLK_PIN, IO_PIN, CE_PIN);

void setup()
{
  Serial.begin(9600);
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

