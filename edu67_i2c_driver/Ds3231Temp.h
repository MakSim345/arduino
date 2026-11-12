#pragma once
#include "I2cDrvIf.h"

/*
 * LAYER 3: THE SENSOR DRIVER
 *
 * Temperature sensor inside the DS3231 RTC.
 * It knows the chip's registers, but NOT how the bus works:
 * it gets an I2cDrvIf& and only calls read()/write().
 * Change the bus driver - this file stays the same.
 */
class Ds3231Temp
{
public:
    explicit Ds3231Temp(I2cDrvIf& bus) : bus(bus) {}

    /** Temperature in 1/100 degree C: 2525 = 25.25 C. */
    bool read(int16_t& centiDegrees)
    {
        // Registers 0x11 and 0x12 in one read: the chip moves to the next register by itself.
        uint8_t raw[2];
        if (!bus.read(ADDRESS, REG_TEMPERATURE, raw, 2))
            return false;

        // 10-bit signed number in quarters of a degree:
        // raw[0] = whole degrees (with sign), raw[1] bits 7..6 = quarters.
        int16_t quarters = (raw[0] << 2) | (raw[1] >> 6);
        if (raw[0] & 0x80)
            quarters -= 1024;           // negative: two's complement of 10 bits
        centiDegrees = quarters * 25;

        // The chip measures only every 64 s. Ask for a new measurement now,
        // so the next read() is fresh. Read-modify-write: keep the other bits.
        uint8_t controlStatus[2];       // 0x0E control, 0x0F status
        if (!bus.read(ADDRESS, REG_CONTROL, controlStatus, 2))
            return false;

        const bool busy = (controlStatus[0] & CONTROL_CONV) || (controlStatus[1] & STATUS_BSY);
        if (!busy)
        {
            const uint8_t control = controlStatus[0] | CONTROL_CONV;
            if (!bus.write(ADDRESS, REG_CONTROL, &control, 1))
                return false;
        }
        return true;
    }

private:
    static const uint8_t ADDRESS         = 0x68;
    static const uint8_t REG_CONTROL     = 0x0E;
    static const uint8_t REG_TEMPERATURE = 0x11;
    static const uint8_t CONTROL_CONV    = 0x20;  // write 1: start a measurement
    static const uint8_t STATUS_BSY      = 0x04;  // 1: measurement running

    I2cDrvIf& bus;
};
