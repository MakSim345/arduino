/*
 * edu67: a layered I2C driver, the way it is done in production firmware.
 *
 * Hardware: Arduino Uno + DS3231 RTC module (ZS-042, with AT24C32 EEPROM).
 *   Module VCC -> 5V, GND -> GND, SDA -> A4, SCL -> A5.
 *
 * Layers (top to bottom):
 *
 *   this sketch
 *      |
 *   Ds3231TempDrv, At24c32Drv   <- sensor drivers: know registers, not the bus
 *      |
 *   I2cDrvIf                    <- the one interface everybody uses
 *      |
 *   I2cWireDrv  or  I2cItDrv    <- bus drivers (choose in Config.h)
 *      |               |
 *   WireIf          TwiHalIf    <- thin wrappers: replaced by fakes in the PC tests
 *      |               |
 *   Wire library    AvrTwiHal   <- the hardware
 *
 * Open the Serial Monitor at 115200 baud. See README.md for the lessons.
 */

#include "Config.h"

#include "At24c32Drv.h"
#include "ClockIf.h"
#include "Ds3231TempDrv.h"
#include "FixedPointFormat.h"
#include "SerialErrorReporter.h"

#if defined(I2C_BACKEND_WIRE)
#include "I2cWireDrv.h"
#include "WireAdapter.h"
#else
#include "AvrTwiHal.h"
#include "I2cItDrv.h"
#endif

constexpr uint32_t I2C_CLOCK_HZ        = 100000;
constexpr uint32_t PRINT_INTERVAL_MS   = 2000;
// 4 bytes starting 2 bytes before a page end: the write is split into two pages.
constexpr uint16_t BOOT_COUNTER_ADDRESS = 0x0F1E;

// Global objects, in the order of their dependencies (constructed top to bottom).
ArduinoClock sysClock;
SerialErrorReporter reporter;

#if defined(I2C_BACKEND_WIRE)
WireAdapter wire;
I2cWireDrv i2cBus(wire);
#else
AvrTwiHal twi;
I2cItDrv i2cBus(twi, sysClock);
#endif

Ds3231TempDrv thermometer(i2cBus);
At24c32Drv eeprom(i2cBus, sysClock);

static uint32_t lastPrintMs = 0;

static void printTemperature(int32_t temperatureE4)
{
    char text[FIXED_E4_TEXT_SIZE];
    formatFixedE4(temperatureE4, text, sizeof(text));
    Serial.print(F("Temperature: "));
    Serial.print(text);
    Serial.println(F(" C"));
}

static void countBoots()
{
    uint8_t bytes[4] = {0, 0, 0, 0};
    if (!eeprom.read(BOOT_COUNTER_ADDRESS, bytes, sizeof(bytes)))
    {
        Serial.println(F("EEPROM read failed"));
        return;
    }

    // Little-endian: least significant byte first.
    uint32_t boots = static_cast<uint32_t>(bytes[0])
                   | (static_cast<uint32_t>(bytes[1]) << 8)
                   | (static_cast<uint32_t>(bytes[2]) << 16)
                   | (static_cast<uint32_t>(bytes[3]) << 24);

    // A new EEPROM is full of 0xFF.
    if (boots == 0xFFFFFFFFul)
    {
        boots = 0;
    }
    ++boots;

    for (uint8_t i = 0; i < 4; ++i)
    {
        bytes[i] = static_cast<uint8_t>(boots >> (8 * i));
    }
    if (!eeprom.write(BOOT_COUNTER_ADDRESS, bytes, sizeof(bytes)))
    {
        Serial.println(F("EEPROM write failed"));
        return;
    }

    Serial.print(F("Boot counter (in EEPROM): "));
    Serial.println(boots);
}


void setup()
{
    Serial.begin(115200);

#if defined(I2C_BACKEND_WIRE)
    Serial.println(F("\nedu67: I2C over the Wire library"));
    wire.begin(I2C_CLOCK_HZ, 25000, false);
#else
    Serial.println(F("\nedu67: I2C over our own TWI interrupt driver"));
    twi.begin(I2C_CLOCK_HZ, false);
#endif

    // The error reporter is connected AFTER the drivers were created:
    // a failure that already happened would be replayed now.
    i2cBus.setErrorReporter(reporter);
    thermometer.setErrorReporter(reporter);
    eeprom.setErrorReporter(reporter);

    Serial.print(F("DS3231 self test: "));
    Serial.println(thermometer.selfTest() ? F("OK") : F("FAILED"));

    countBoots();
}

void loop()
{
    // Non-blocking timing, as in edu01_blink_no_delay.
    const uint32_t now = millis();
    if (now - lastPrintMs < PRINT_INTERVAL_MS)
    {
        return;
    }
    lastPrintMs = now;

    int32_t temperatureE4 = 0;
    if (thermometer.getTemperature(temperatureE4))
    {
        printTemperature(temperatureE4);
    }
    else
    {
        Serial.println(F("Temperature: no sensor"));
    }
}
