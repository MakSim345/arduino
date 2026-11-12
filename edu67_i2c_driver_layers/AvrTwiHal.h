#ifndef EDU67_AVRTWIHAL_H
#define EDU67_AVRTWIHAL_H

#include "Config.h"
#include "TwiHalIf.h"

#if defined(ARDUINO_ARCH_AVR) && defined(I2C_BACKEND_IT)

/**
 * TWI hardware of the ATmega328P (Uno, Nano), driven by its interrupt.
 *
 * Each time the TWI finishes one step (START sent, address ACKed, byte
 * received, ...) it sets TWINT and calls ISR(TWI_vect). The ISR reads the
 * status code in TWSR and decides the next step: a state machine.
 * See ATmega328P datasheet, chapter "2-wire Serial Interface".
 *
 * There is one TWI, so there may be only one AvrTwiHal object.
 */
class AvrTwiHal : public TwiHalIf
{
public:
    AvrTwiHal();

    /**
     * @param[in] clockHzP          SCL frequency, e.g. 100000.
     * @param[in] internalPullupsP  See WireAdapter::begin().
     */
    void begin(uint32_t clockHzP, bool internalPullupsP);

    bool memReadIt(
        uint8_t deviceAddress8bitP,
        uint16_t registerAddressP,
        uint8_t registerAddressSizeP,
        uint8_t* pDataP,
        uint16_t dataSizeP) override;

    bool memWriteIt(
        uint8_t deviceAddress8bitP,
        uint16_t registerAddressP,
        uint8_t registerAddressSizeP,
        const uint8_t* pDataP,
        uint16_t dataSizeP) override;

    void abort() override;
    void setCallbacks(CallbacksIf& rCallbacksP) override;
    void resetCallbacks(CallbacksIf& rCallbacksP) override;
    I2cResult getError() const override;

    /** One step of the state machine. Called ONLY from ISR(TWI_vect). */
    void handleInterrupt();

private:
    enum class Operation : uint8_t
    {
        IDLE,
        MEM_READ,
        MEM_WRITE
    };

    /** Common part of memReadIt() / memWriteIt(). */
    bool start(
        Operation operationP,
        uint8_t deviceAddress8bitP,
        uint16_t registerAddressP,
        uint8_t registerAddressSizeP,
        uint16_t dataSizeP);

    /** Ends the transfer and calls the callback. */
    void finish(I2cResult resultP);

    volatile Operation operationM;
    uint8_t address8bitM;
    uint8_t registerBytesM[2];
    uint8_t registerIndexM; ///< 0 for a 2-byte register address, 1 for a 1-byte one; done at 2

    uint8_t* pRxDataM;
    const uint8_t* pTxDataM;
    uint16_t dataSizeM;
    uint16_t dataIndexM;
    volatile I2cResult errorM;
    CallbacksIf* volatile pCallbacksM;
};

#endif

#endif // EDU67_AVRTWIHAL_H
