#ifndef EDU67_TWIHALIF_H
#define EDU67_TWIHALIF_H

#include "I2cDrvIf.h"

/**
 * Interrupt-driven TWI hardware ("HAL" = Hardware Abstraction Layer).
 *
 * The functions only START a transfer and return at once. The transfer
 * continues in the TWI interrupt, byte by byte, and when it is over one of
 * the callbacks is called - FROM THE INTERRUPT.
 *
 * Like the STM32 HAL, this layer takes the 8-bit address:
 * (7-bit address << 1). Mixing up 7-bit and 8-bit addresses is the classic
 * I2C bug, so the conversion happens in exactly one place (I2cItDrv).
 */
class TwiHalIf
{
public:
    /** Implemented by whoever wants to know when a transfer is over. */
    class CallbacksIf
    {
    public:
        // NOTE: all callbacks are called from the interrupt. Keep them short!
        virtual void memRxCpltCallback() = 0;
        virtual void memTxCpltCallback() = 0;
        virtual void errorCallback() = 0;

    protected:
        ~CallbacksIf() {}
    };

    virtual ~TwiHalIf() {}

    /** Starts a register read. @return false if the hardware is busy. */
    virtual bool memReadIt(
        uint8_t deviceAddress8bitP,
        uint16_t registerAddressP,
        uint8_t registerAddressSizeP,
        uint8_t* pDataP,
        uint16_t dataSizeP) = 0;

    /** Starts a register write. @return false if the hardware is busy. */
    virtual bool memWriteIt(
        uint8_t deviceAddress8bitP,
        uint16_t registerAddressP,
        uint8_t registerAddressSizeP,
        const uint8_t* pDataP,
        uint16_t dataSizeP) = 0;

    /** Stops the running transfer and resets the hardware. No callback follows. */
    virtual void abort() = 0;

    virtual void setCallbacks(CallbacksIf& rCallbacksP) = 0;
    virtual void resetCallbacks(CallbacksIf& rCallbacksP) = 0;

    /** @return why the last transfer failed (valid in errorCallback()). */
    virtual I2cResult getError() const = 0;
};

#endif // EDU67_TWIHALIF_H
