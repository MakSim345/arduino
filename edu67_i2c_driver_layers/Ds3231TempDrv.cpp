#include "Ds3231TempDrv.h"

#include "LockGuard.h"

namespace {

constexpr uint16_t REGISTER_ADDRESS_SIZE = 1;

// One LSB of the 10-bit temperature is 0.25 C = 2500 * (1/10000 C).
constexpr int32_t LSB_E4 = 2500;

} // anonymous namespace

Ds3231TempDrv::Ds3231TempDrv(I2cDrvIf& rBusP, uint8_t addressP) :
    rBusM(rBusP),
    addressM(addressP),
    latchM(Subsystem::PERIPHERALS, HwError::DS3231_FAILURE)
{
}

bool Ds3231TempDrv::init()
{
    if (latchM.isSet())
    {
        return false;
    }

    uint8_t control = 0;
    if (!rBusM.read(addressM, DS3231_REG_CONTROL, REGISTER_ADDRESS_SIZE, &control, 1))
    {
        return fail(EDU_TEXT("DS3231 does not answer, address"));
    }
    return true;
}

bool Ds3231TempDrv::getTemperature(int32_t& rTemperatureE4P)
{
    if (latchM.isSet())
    {
        return false;
    }

    // Hold the bus for the whole read + check + trigger sequence,
    // so nobody else can talk to the chip in between.
    const LockGuard<I2cDrvIf> lock(rBusM);

    // Two registers in one transfer: the chip increments the register address itself.
    uint8_t raw[2] = {0, 0};
    if (!rBusM.read(addressM, DS3231_REG_TEMP_MSB, REGISTER_ADDRESS_SIZE, raw, sizeof(raw)))
    {
        return fail(EDU_TEXT("DS3231 temperature read failed, address"));
    }

    // 10-bit two's complement number, in units of 0.25 C:
    // - raw[0] holds bits 9..2 (the whole degrees, with the sign)
    // - raw[1] bits 7..6 hold bits 1..0 (the quarters)
    int16_t quarters = static_cast<int16_t>((raw[0] << 2) | (raw[1] >> 6));

    // Negative number: bit 9 is the sign. Sign-extend 10 bits to 16.
    if (raw[0] & 0x80)
    {
        quarters = static_cast<int16_t>(quarters - 0x400);
    }

    // int32_t is not optional: on AVR "int" is 16 bits (max 32767),
    // and already 25 C = 100 quarters * 2500 = 250000 would overflow.
    rTemperatureE4P = static_cast<int32_t>(quarters) * LSB_E4;

    // Start a new conversion, but only if none is running.
    uint8_t controlStatus[2] = {0, 0}; // 0x0E control, 0x0F status
    if (!rBusM.read(addressM, DS3231_REG_CONTROL, REGISTER_ADDRESS_SIZE, controlStatus, sizeof(controlStatus)))
    {
        return fail(EDU_TEXT("DS3231 control read failed, address"));
    }

    const bool isConverting =
        (controlStatus[0] & DS3231_CONTROL_CONV) ||
        (controlStatus[1] & DS3231_STATUS_BSY);

    if (!isConverting)
    {
        // Read-modify-write: keep all other bits of the control register as they are.
        const uint8_t control = controlStatus[0] | DS3231_CONTROL_CONV;
        if (!rBusM.write(addressM, DS3231_REG_CONTROL, REGISTER_ADDRESS_SIZE, &control, 1))
        {
            return fail(EDU_TEXT("DS3231 conversion start failed, address"));
        }
    }

    return true;
}

bool Ds3231TempDrv::selfTest()
{
    constexpr int32_t MIN_TEMPERATURE_E4 = -40L * 10000L;
    constexpr int32_t MAX_TEMPERATURE_E4 =  85L * 10000L;

    if (!init())
    {
        return false;
    }

    int32_t temperatureE4 = 0;
    if (!getTemperature(temperatureE4))
    {
        return false;
    }

    if ((temperatureE4 < MIN_TEMPERATURE_E4) || (temperatureE4 > MAX_TEMPERATURE_E4))
    {
        latchM.log(EDU_TEXT("DS3231 temperature out of range, C"), static_cast<uint16_t>(temperatureE4 / 10000));
        return false;
    }
    return true;
}

bool Ds3231TempDrv::fail(EduText messageP)
{
    if (!latchM.isSet())
    {
        latchM.log(messageP, addressM);
        latchM.set();
    }
    return false;
}
