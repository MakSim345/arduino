/*
 * edu67: how an I2C driver is built - interface, driver, interrupt.
 *
 * Hardware: Arduino Uno + DS3231 RTC module
 *   SDA -> A4, SCL -> A5, VCC -> 5V, GND -> GND
 *
 * Files (tabs):
 *   I2cDrvIf.h           LAYER 1  interface: read() / write()
 *   I2cDrv.h, I2cDrv.cpp LAYER 2  driver: TWI registers + ISR(TWI_vect)
 *   Ds3231Temp.h         LAYER 3  sensor: knows registers, uses only I2cDrvIf
 *   this file                     the application
 *
 * The same pieces in the STM32 project (sensor-arm):
 *   I2cDrvIf.h            -> Msf/Hardware/MicroController/I2cDrvIf.hpp
 *   I2cDrv::startAndWait  -> Stm32L4/I2cDrv.cpp: HAL_I2C_Mem_Read_IT + eventQueue.tryReceive(100 ms)
 *   I2cDrv::onInterrupt   -> the STM32 HAL interrupt code (hidden in the HAL library)
 *   finish(): result = .. -> HAL_I2C_MemRxCpltCallback -> eventQueue.trySendFromIsr(RX_COMPLETE)
 *   Ds3231Temp            -> Msf/Hardware/Peripherals/Tmp112Drv.cpp
 *
 * Do NOT include Wire.h here: the Wire library has its own ISR(TWI_vect).
 */

#include "I2cDrv.h"
#include "Ds3231Temp.h"

I2cDrv i2c;                   // the real driver...
Ds3231Temp thermometer(i2c);  // ...given to the sensor as an I2cDrvIf&

void printCentiDegrees(int16_t value)
{
    if (value < 0)
    {
        Serial.print('-');    // print the sign ourselves: -25 / 100 = 0, the '-' would be lost
        value = -value;
    }
    Serial.print(value / 100);
    Serial.print('.');
    if (value % 100 < 10)
        Serial.print('0');
    Serial.print(value % 100);
    Serial.println(F(" C"));
}

void setup()
{
    Serial.begin(115200);
    i2c.begin(100000);
    Serial.println(F("edu67: I2C driver with interrupts"));
}

void loop()
{
    int16_t temperature;
    if (thermometer.read(temperature))
    {
        Serial.print(F("Temperature: "));
        printCentiDegrees(temperature);
    }
    else
    {
        Serial.println(F("DS3231 does not answer"));
    }
    delay(2000);
}
