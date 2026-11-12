#include "I2cItDrv.h"

#include "LockGuard.h"

I2cItDrv::I2cItDrv(
    TwiHalIf& rHalP,
    ClockIf& rClockP,
    uint16_t timeoutMsP
) :
    rHalM(rHalP),
    rClockM(rClockP),
    timeoutMsM(timeoutMsP)
{
    rHalM.setCallbacks(*this);
}

I2cItDrv::~I2cItDrv()
{
    rHalM.resetCallbacks(*this);
}

bool I2cItDrv::read(
    uint16_t deviceAddressP,
    uint16_t registerAddressP,
    uint16_t registerAddressSizeP,
    uint8_t* pDataP,
    size_t dataSizeP)
{
    checkRequest(deviceAddressP, registerAddressSizeP, pDataP);
    EDU_ASSERT((dataSizeP > 0) && (dataSizeP <= 0xFFFF));
    // With interrupts off, the "complete" interrupt can never come (and millis() stops).
    EDU_ASSERT(eduInterruptsEnabled());

    const LockGuard<I2cItDrv> lock(*this);

    // An event left over from an earlier, timed-out transfer must not
    // be mistaken for the end of this one.
    eventSlotM.clear();

    // The only place where the 7-bit address becomes the 8-bit one.
    if (!rHalM.memReadIt(
        static_cast<uint8_t>(deviceAddressP << 1),
        registerAddressP,
        static_cast<uint8_t>(registerAddressSizeP),
        pDataP,
        static_cast<uint16_t>(dataSizeP)))
    {
        return fail(rHalM.getError());
    }

    return waitForCompletion(EVENT_RX_COMPLETE);
}

bool I2cItDrv::write(
    uint16_t deviceAddressP,
    uint16_t registerAddressP,
    uint16_t registerAddressSizeP,
    const uint8_t* pDataP,
    size_t dataSizeP)
{
    checkRequest(deviceAddressP, registerAddressSizeP, pDataP);
    EDU_ASSERT(dataSizeP <= 0xFFFF);
    EDU_ASSERT(eduInterruptsEnabled());

    const LockGuard<I2cItDrv> lock(*this);

    eventSlotM.clear();

    if (!rHalM.memWriteIt(
        static_cast<uint8_t>(deviceAddressP << 1),
        registerAddressP,
        static_cast<uint8_t>(registerAddressSizeP),
        pDataP,
        static_cast<uint16_t>(dataSizeP)))
    {
        return fail(rHalM.getError());
    }

    return waitForCompletion(EVENT_TX_COMPLETE);
}

bool I2cItDrv::waitForCompletion(Event expectedP)
{
    uint8_t event  = 0;
    uint8_t detail = 0;

    if (!eventSlotM.tryReceive(event, detail, rClockM, timeoutMsM))
    {
        // The interrupt never finished the transfer. Stop the hardware
        // (it still points into the caller's buffer!) and drop anything
        // that sneaked in meanwhile.
        rHalM.abort();
        eventSlotM.clear();
        return fail(I2cResult::TIMEOUT);
    }

    if (event == expectedP)
    {
        return succeed();
    }
    if (event == EVENT_ERROR)
    {
        return fail(static_cast<I2cResult>(detail));
    }

    // RX_COMPLETE for a write, or the other way round: a bug somewhere.
    log(EDU_TEXT("I2C unexpected event"), event);
    return fail(I2cResult::BUS_ERROR);
}

void I2cItDrv::memRxCpltCallback()
{
    eventSlotM.trySendFromIsr(EVENT_RX_COMPLETE, 0);
}

void I2cItDrv::memTxCpltCallback()
{
    eventSlotM.trySendFromIsr(EVENT_TX_COMPLETE, 0);
}

void I2cItDrv::errorCallback()
{
    eventSlotM.trySendFromIsr(EVENT_ERROR, static_cast<uint8_t>(rHalM.getError()));
}
