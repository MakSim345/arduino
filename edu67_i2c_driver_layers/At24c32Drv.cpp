#include "At24c32Drv.h"

#include "LockGuard.h"

namespace {

constexpr uint16_t MEMORY_ADDRESS_SIZE = 2;

// Not "min": Arduino.h defines min() as a macro.
size_t smaller(size_t aP, size_t bP)
{
    return (aP < bP) ? aP : bP;
}

} // anonymous namespace

At24c32Drv::At24c32Drv(I2cDrvIf& rBusP, ClockIf& rClockP, uint8_t addressP) :
    rBusM(rBusP),
    rClockM(rClockP),
    addressM(addressP),
    latchM(Subsystem::PERIPHERALS, HwError::AT24C32_FAILURE)
{
}

bool At24c32Drv::read(uint16_t memoryAddressP, uint8_t* pDataP, size_t dataSizeP)
{
    EDU_ASSERT(pDataP != nullptr);
    EDU_ASSERT(memoryAddressP + dataSizeP <= AT24C32_SIZE);

    if (latchM.isSet())
    {
        return false;
    }

    const LockGuard<I2cDrvIf> lock(rBusM);

    // Reads may cross pages; they are split only to fit the bus buffer.
    while (dataSizeP > 0)
    {
        const size_t chunk = smaller(dataSizeP, AT24C32_CHUNK_SIZE);
        if (!rBusM.read(addressM, memoryAddressP, MEMORY_ADDRESS_SIZE, pDataP, chunk))
        {
            return fail(EDU_TEXT("AT24C32 read failed at"), memoryAddressP);
        }
        memoryAddressP = static_cast<uint16_t>(memoryAddressP + chunk);
        pDataP += chunk;
        dataSizeP -= chunk;
    }
    return true;
}

bool At24c32Drv::write(uint16_t memoryAddressP, const uint8_t* pDataP, size_t dataSizeP)
{
    EDU_ASSERT(pDataP != nullptr);
    EDU_ASSERT(memoryAddressP + dataSizeP <= AT24C32_SIZE);

    if (latchM.isSet())
    {
        return false;
    }

    const LockGuard<I2cDrvIf> lock(rBusM);

    while (dataSizeP > 0)
    {
        // Example: address 30, 4 bytes -> 2 bytes to the end of page 0, then 2 bytes in page 1.
        const size_t toPageEnd = AT24C32_PAGE_SIZE - (memoryAddressP % AT24C32_PAGE_SIZE);
        const size_t chunk     = smaller(smaller(dataSizeP, toPageEnd), AT24C32_CHUNK_SIZE);

        if (!rBusM.write(addressM, memoryAddressP, MEMORY_ADDRESS_SIZE, pDataP, chunk))
        {
            return fail(EDU_TEXT("AT24C32 write failed at"), memoryAddressP);
        }
        if (!waitWhileBusy())
        {
            return fail(EDU_TEXT("AT24C32 write cycle timeout at"), memoryAddressP);
        }
        memoryAddressP = static_cast<uint16_t>(memoryAddressP + chunk);
        pDataP += chunk;
        dataSizeP -= chunk;
    }
    return true;
}

bool At24c32Drv::waitWhileBusy()
{
    // While writing its cells the chip NACKs its address. The first transfer
    // that is ACKed tells us it is ready. (A zero-length write is the textbook
    // way; a 1-byte read works with every bus driver, and is harmless.)
    const uint32_t startMs = rClockM.nowMs();
    uint8_t dummy = 0;
    do
    {
        if (rBusM.read(addressM, 0, MEMORY_ADDRESS_SIZE, &dummy, 1))
        {
            return true;
        }
    } while (static_cast<uint32_t>(rClockM.nowMs() - startMs) < AT24C32_WRITE_TIMEOUT_MS);

    return false;
}

bool At24c32Drv::fail(EduText messageP, uint16_t memoryAddressP)
{
    if (!latchM.isSet())
    {
        latchM.log(messageP, memoryAddressP);
        latchM.set();
    }
    return false;
}
