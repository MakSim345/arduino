#include "I2cDrvBase.h"

I2cDrvBase::I2cDrvBase() :
    latchM(Subsystem::I2C_BUS, HwError::I2C_FAILURE),
    lastResultM(I2cResult::OK)
{
}

void I2cDrvBase::checkRequest(
    uint16_t deviceAddressP,
    uint16_t registerAddressSizeP,
    const void* pDataP)
{
    // A 7-bit address is shifted left by one bit on the wire (the lowest bit is R/W).
    // Anything above 0x7F would lose its top bit - usually it means somebody
    // passed an already shifted "8-bit" address from a datasheet.
    EDU_ASSERT(deviceAddressP <= I2C_MAX_DEVICE_ADDRESS);
    EDU_ASSERT(
        (registerAddressSizeP >= I2C_MIN_REGISTER_ADDRESS_SIZE) &&
        (registerAddressSizeP <= I2C_MAX_REGISTER_ADDRESS_SIZE));
    EDU_ASSERT(pDataP != nullptr);
}

bool I2cDrvBase::succeed()
{
    lastResultM = I2cResult::OK;
    return true;
}

bool I2cDrvBase::fail(I2cResult resultP)
{
    EDU_ASSERT(resultP != I2cResult::OK);
    lastResultM = resultP;

    switch (resultP)
    {
    case I2cResult::NACK_ADDRESS:
    case I2cResult::NACK_DATA:
        // Not our business: the device driver decides what a NACK means.
        break;

    case I2cResult::ARBITRATION_LOST:
        // Only possible with several masters: retrying later is fine.
        log(EDU_TEXT("I2C arbitration lost"), 0);
        break;

    default:
        log(EDU_TEXT("I2C bus failure, result"), static_cast<uint16_t>(resultP));
        latchM.set();
        break;
    }
    return false;
}
