#pragma once
#include <Arduino.h>

/*
 * LAYER 1: THE INTERFACE
 *
 * What every I2C driver can do - and nothing about HOW.
 * A sensor driver sees only this class. It does not know if underneath
 * there is our interrupt driver, the Wire library, or an STM32.
 *
 * "= 0" means pure virtual: there is no code here, a real driver
 * (I2cDrv) must provide it.
 *
 * read():  START | addr+W | reg | REPEATED START | addr+R | data... | STOP
 * write(): START | addr+W | reg | data...                          | STOP
 */
class I2cDrvIf
{
public:
    virtual ~I2cDrvIf() {}

    /**
     * Read 'size' bytes from register 'reg' of the device.
     * deviceAddress is the 7-bit address from the datasheet (e.g. 0x68).
     * Blocking: returns when done. true = OK, false = error or timeout.
     */
    virtual bool read(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t size) = 0;

    /** Write 'size' bytes to register 'reg' of the device. */
    virtual bool write(uint8_t deviceAddress, uint8_t reg, const uint8_t* data, uint8_t size) = 0;
};
