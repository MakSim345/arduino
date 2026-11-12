#ifndef EDU67_AT24C32DRV_H
#define EDU67_AT24C32DRV_H

#include "ClockIf.h"
#include "FatalErrorLatch.h"
#include "I2cDrvIf.h"

/** Address on the ZS-042 DS3231 module (A0..A2 pulled up). Other modules: 0x50..0x57. */
constexpr uint8_t AT24C32_ADDRESS = 0x57;

constexpr uint16_t AT24C32_SIZE      = 4096; ///< 32 kbit = 4096 bytes
constexpr uint16_t AT24C32_PAGE_SIZE = 32;   ///< one write may not cross a page boundary

/**
 * Bytes per bus transfer. Wire has a 32-byte buffer and the 2-byte memory
 * address takes 2 of them, so 16 is safe for every bus driver.
 */
constexpr uint16_t AT24C32_CHUNK_SIZE = 16;

/** Write cycle is max 10 ms by datasheet; a bit more for safety. */
constexpr uint16_t AT24C32_WRITE_TIMEOUT_MS = 20;

/**
 * AT24C32 I2C EEPROM.
 *
 * New compared to the DS3231:
 * - 2-byte "register" address (0x000..0xFFF): registerAddressSize = 2.
 * - Page writes: bytes that cross the end of a 32-byte page WRAP AROUND to
 *   the start of the same page. So a long write must be split at page ends.
 * - Write cycle: after a write, the chip is busy for up to 10 ms and does not
 *   ACK its own address. We ask again until it answers ("ACK polling"),
 *   which is faster than always waiting 10 ms.
 */
class At24c32Drv
{
public:
    At24c32Drv(I2cDrvIf& rBusP, ClockIf& rClockP, uint8_t addressP = AT24C32_ADDRESS);

    At24c32Drv(const At24c32Drv&) = delete;
    At24c32Drv& operator=(const At24c32Drv&) = delete;

    void setErrorReporter(ErrorReporterIf& rReporterP) { latchM.setReporter(rReporterP); }
    bool hasFatalFailure() const { return latchM.isSet(); }

    bool read(uint16_t memoryAddressP, uint8_t* pDataP, size_t dataSizeP);
    bool write(uint16_t memoryAddressP, const uint8_t* pDataP, size_t dataSizeP);

private:
    /** ACK polling until the write cycle is over. */
    bool waitWhileBusy();

    bool fail(EduText messageP, uint16_t memoryAddressP);

    I2cDrvIf& rBusM;
    ClockIf& rClockM;
    const uint8_t addressM;
    FatalErrorLatch latchM;
};

#endif // EDU67_AT24C32DRV_H
