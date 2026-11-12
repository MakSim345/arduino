#ifndef EDU67_I2CWIREDRV_H
#define EDU67_I2CWIREDRV_H

#include "I2cDrvBase.h"
#include "WireIf.h"

/**
 * Lesson 2: blocking I2C driver on top of the Wire library.
 *
 * Wire only knows "send some bytes" and "receive some bytes". This driver
 * builds the register read/write protocol from them, checks the limits of
 * the Wire buffer, and turns Wire status codes into I2cResult.
 */
class I2cWireDrv : public I2cDrvBase
{
public:
    explicit I2cWireDrv(WireIf& rWireP);

    /** @see I2cDrvIf::read */
    bool read(
        uint16_t deviceAddressP,
        uint16_t registerAddressP,
        uint16_t registerAddressSizeP,
        uint8_t* pDataP,
        size_t dataSizeP) override;

    /** @see I2cDrvIf::write */
    bool write(
        uint16_t deviceAddressP,
        uint16_t registerAddressP,
        uint16_t registerAddressSizeP,
        const uint8_t* pDataP,
        size_t dataSizeP) override;

private:
    /** Queues the register address, most significant byte first. */
    void writeRegisterAddress(uint16_t registerAddressP, uint16_t registerAddressSizeP);

    /** Translates a Wire status code at the boundary, so nobody else has to know them. */
    static I2cResult toI2cResult(uint8_t wireStatusP);

    WireIf& rWireM;
};

#endif // EDU67_I2CWIREDRV_H
