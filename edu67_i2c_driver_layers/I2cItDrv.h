#ifndef EDU67_I2CITDRV_H
#define EDU67_I2CITDRV_H

#include "ClockIf.h"
#include "EventSlot.h"
#include "I2cDrvBase.h"
#include "TwiHalIf.h"

/** Default timeout of one read or write. */
constexpr uint16_t I2C_IT_DEFAULT_TIMEOUT_MS = 100;

/**
 * Lesson 3: I2C driver on top of an interrupt-driven HAL.
 *
 * The HAL is asynchronous: "start the transfer, I will call you back".
 * This driver turns that into a simple blocking read()/write():
 *
 *   main code                         TWI interrupt
 *   ---------                         -------------
 *   read():
 *     slot.clear()
 *     hal.memReadIt()   -----------> state machine runs, byte by byte
 *     slot.tryReceive(timeout)            ...
 *        (waiting)      <-----------  memRxCpltCallback(): slot.trySendFromIsr(RX_COMPLETE)
 *     return true
 *
 * Why is the blocking wait still useful? The caller's buffer lives on its
 * stack. read() must not return while the interrupt may still write into it.
 */
class I2cItDrv :
    public I2cDrvBase,
    private TwiHalIf::CallbacksIf
{
public:
    I2cItDrv(
        TwiHalIf& rHalP,
        ClockIf& rClockP,
        uint16_t timeoutMsP = I2C_IT_DEFAULT_TIMEOUT_MS);
    ~I2cItDrv() override;

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
    /** Events from the interrupt to the main code. */
    enum Event : uint8_t
    {
        EVENT_RX_COMPLETE = 1,
        EVENT_TX_COMPLETE = 2,
        EVENT_ERROR       = 3
    };

    /** @see TwiHalIf::CallbacksIf. Called from the interrupt! */
    void memRxCpltCallback() override;
    void memTxCpltCallback() override;
    void errorCallback() override;

    /** Waits for the end of the transfer that was just started. */
    bool waitForCompletion(Event expectedP);

    TwiHalIf& rHalM;
    ClockIf& rClockM;
    EventSlot eventSlotM;
    const uint16_t timeoutMsM;
};

#endif // EDU67_I2CITDRV_H
