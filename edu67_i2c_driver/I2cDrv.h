#pragma once
#include "I2cDrvIf.h"

/*
 * LAYER 2: THE DRIVER
 *
 * Implements I2cDrvIf with the TWI hardware of the ATmega328P (Uno, Nano).
 * No Wire library: we program the TWI registers ourselves.
 *
 * The idea (same as a production STM32 driver):
 *   - read()/write() only START the transfer, then WAIT
 *   - the TWI interrupt moves the bytes, one step per interrupt
 *   - when the transfer is over, the interrupt sets 'result'
 *   - read()/write() see it and return; after 100 ms they give up
 */
class I2cDrv : public I2cDrvIf
{
public:
    void begin(uint32_t clockHz = 100000);

    bool read(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t size) override;
    bool write(uint8_t deviceAddress, uint8_t reg, const uint8_t* data, uint8_t size) override;

    /** One step of the transfer. Called ONLY from ISR(TWI_vect). */
    void onInterrupt();

private:
    enum Result : uint8_t { BUSY, DONE, FAILED };

    bool startAndWait();
    void finish(Result r);

    // The transfer: set by read()/write(), used by the interrupt.
    uint8_t address;
    uint8_t reg;
    bool isRead;
    uint8_t* rxData;
    const uint8_t* txData;
    uint8_t size;
    uint8_t index;

    // Set by the interrupt, read by the main code.
    // 'volatile' = "can change behind your back": the compiler must
    // read it from memory every time, not keep an old copy.
    volatile Result result;
};
