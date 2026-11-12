#include "I2cWireDrv.h"

#include "LockGuard.h"

I2cWireDrv::I2cWireDrv(WireIf& rWireP) :
    rWireM(rWireP)
{
}

bool I2cWireDrv::read(
    uint16_t deviceAddressP,
    uint16_t registerAddressP,
    uint16_t registerAddressSizeP,
    uint8_t* pDataP,
    size_t dataSizeP)
{
    checkRequest(deviceAddressP, registerAddressSizeP, pDataP);
    EDU_ASSERT(dataSizeP > 0);

    const LockGuard<I2cWireDrv> lock(*this);

    if (dataSizeP > rWireM.bufferSize())
    {
        return fail(I2cResult::TOO_LONG);
    }

    const uint8_t address = static_cast<uint8_t>(deviceAddressP);

    // Step 1: tell the device WHICH register we want to read.
    // No STOP at the end: we keep the bus, so nobody can change the register pointer.
    rWireM.beginTransmission(address);
    writeRegisterAddress(registerAddressP, registerAddressSizeP);
    const uint8_t status = rWireM.endTransmission(false);
    if (status != WIRE_OK)
    {
        return fail(toI2cResult(status));
    }

    // Step 2: REPEATED START, read the data, STOP.
    const uint8_t count = static_cast<uint8_t>(dataSizeP);
    if (rWireM.requestFrom(address, count, true) != count)
    {
        // Wire does not say why. Timeout is the only other thing it tracks.
        return fail(rWireM.takeTimeoutFlag() ? I2cResult::TIMEOUT : I2cResult::NACK_ADDRESS);
    }

    for (size_t i = 0; i < dataSizeP; ++i)
    {
        pDataP[i] = static_cast<uint8_t>(rWireM.read());
    }
    return succeed();
}

bool I2cWireDrv::write(
    uint16_t deviceAddressP,
    uint16_t registerAddressP,
    uint16_t registerAddressSizeP,
    const uint8_t* pDataP,
    size_t dataSizeP)
{
    checkRequest(deviceAddressP, registerAddressSizeP, pDataP);

    const LockGuard<I2cWireDrv> lock(*this);

    // The register address takes room in the same buffer:
    // with a 32-byte buffer and a 2-byte EEPROM address, at most 30 data bytes fit.
    if (registerAddressSizeP + dataSizeP > rWireM.bufferSize())
    {
        return fail(I2cResult::TOO_LONG);
    }

    rWireM.beginTransmission(static_cast<uint8_t>(deviceAddressP));
    writeRegisterAddress(registerAddressP, registerAddressSizeP);
    for (size_t i = 0; i < dataSizeP; ++i)
    {
        rWireM.write(pDataP[i]);
    }

    const uint8_t status = rWireM.endTransmission(true);
    if (status != WIRE_OK)
    {
        return fail(toI2cResult(status));
    }
    return succeed();
}

void I2cWireDrv::writeRegisterAddress(uint16_t registerAddressP, uint16_t registerAddressSizeP)
{
    if (registerAddressSizeP == 2)
    {
        rWireM.write(static_cast<uint8_t>(registerAddressP >> 8));
    }
    rWireM.write(static_cast<uint8_t>(registerAddressP & 0xFF));
}

I2cResult I2cWireDrv::toI2cResult(uint8_t wireStatusP)
{
    switch (wireStatusP)
    {
    case WIRE_DATA_TOO_LONG: return I2cResult::TOO_LONG;
    case WIRE_NACK_ADDRESS:  return I2cResult::NACK_ADDRESS;
    case WIRE_NACK_DATA:     return I2cResult::NACK_DATA;
    case WIRE_TIMEOUT:       return I2cResult::TIMEOUT;
    default:                 return I2cResult::BUS_ERROR;
    }
}
