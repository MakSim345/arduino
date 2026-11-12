#ifndef EDU67_I2CDRVIF_H
#define EDU67_I2CDRVIF_H

#include "Platform.h"

/** 7-bit addresses only: 0x00..0x7F. */
constexpr uint16_t I2C_MAX_DEVICE_ADDRESS = 0x7F;

/** Size of the register ("internal" / "memory") address: 1 byte (sensors) or 2 bytes (EEPROMs). */
constexpr uint16_t I2C_MIN_REGISTER_ADDRESS_SIZE = 1;
constexpr uint16_t I2C_MAX_REGISTER_ADDRESS_SIZE = 2;

/** Result of the last bus operation, the same for every bus driver. */
enum class I2cResult : uint8_t
{
    OK,
    NACK_ADDRESS,     ///< Nobody answered: wrong address, not connected, or busy (EEPROM write cycle)
    NACK_DATA,        ///< The device refused a byte
    TOO_LONG,         ///< The transfer does not fit into the driver buffer
    TIMEOUT,          ///< The bus is stuck (SDA or SCL held low)
    BUS_ERROR,        ///< Illegal START/STOP seen on the bus: noise or bad wiring
    ARBITRATION_LOST, ///< Another master won the bus
    BUSY              ///< The hardware was still busy with a previous transfer
};

/**
 * A general interface for an I2C bus driver.
 *
 * Sensor drivers only know this interface. They do not know if the bus is the
 * Wire library, our own interrupt-driven driver, or a fake from a unit test.
 *
 * read() and write() are "register" (memory) transfers, the way almost every
 * I2C chip works:
 *
 *   read:  START | addr+W | register | REPEATED START | addr+R | data... | STOP
 *   write: START | addr+W | register | data...                          | STOP
 *
 * Locking: the bus is shared. read()/write() lock it internally, but a driver
 * that needs several transfers in a row without anybody else in between
 * (read-modify-write) must hold the lock around the whole sequence:
 *
 *   const LockGuard<I2cDrvIf> lock(rBus);
 *   rBus.read(...);
 *   rBus.write(...);
 *
 * All operations are blocking: they return when the transfer is finished,
 * failed, or timed out.
 */
class I2cDrvIf
{
public:
    virtual ~I2cDrvIf() {}

    /** Take the bus (recursive: the same owner may lock again). */
    virtual void lock() = 0;

    /** Release the bus. */
    virtual void unlock() = 0;

    /**
     * Read from a register of an I2C device.
     *
     * @param[in]  deviceAddressP        7-bit device address (as in the datasheet, NOT shifted).
     * @param[in]  registerAddressP      Register inside the device.
     * @param[in]  registerAddressSizeP  1 or 2 bytes.
     * @param[out] pDataP                Buffer for the data.
     * @param[in]  dataSizeP             How many bytes to read (at least 1).
     * @return true if successful, false if something went wrong.
     */
    virtual bool read(
        uint16_t deviceAddressP,
        uint16_t registerAddressP,
        uint16_t registerAddressSizeP,
        uint8_t* pDataP,
        size_t dataSizeP) = 0;

    /**
     * Write to a register of an I2C device. Same parameters as read().
     * dataSizeP may be 0: then only the register address is sent.
     */
    virtual bool write(
        uint16_t deviceAddressP,
        uint16_t registerAddressP,
        uint16_t registerAddressSizeP,
        const uint8_t* pDataP,
        size_t dataSizeP) = 0;
};

#endif // EDU67_I2CDRVIF_H
