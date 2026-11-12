#include "I2cDrv.h"
#include <util/twi.h>   // TW_STATUS and names of the TWI status codes

/*
 * How a read of 2 bytes from register 0x11 of device 0x68 goes:
 *
 *   main code (read)                    TWI interrupt (onInterrupt)
 *   ----------------                    ---------------------------
 *   fill address/reg/size
 *   TWCR = START  ------------------->  TW_START       : send 0x68+W
 *   wait for result != BUSY             TW_MT_SLA_ACK  : send 0x11
 *     ...                               TW_MT_DATA_ACK : REPEATED START
 *     ...                               TW_REP_START   : send 0x68+R
 *     ...                               TW_MR_SLA_ACK  : receive byte 0, ACK it
 *     ...                               TW_MR_DATA_ACK : store byte 0, receive byte 1, NACK it
 *     ...          <------------------  TW_MR_DATA_NACK: store byte 1, STOP, result = DONE
 *   return true
 *
 * Each line on the right is one interrupt. The hardware sets the TWINT flag
 * when it has finished a step, and TWSR tells which step it was.
 * (ATmega328P datasheet, chapter "2-wire Serial Interface".)
 */

static const uint16_t TIMEOUT_MS = 100;

// The ISR is a plain function, it needs a pointer to the driver object.
static I2cDrv* instance = nullptr;

ISR(TWI_vect)
{
    instance->onInterrupt();
}

// Writing TWCR with TWINT=1 clears the flag: "hardware, do the next step".
// The other bits choose which step.
#define TWCR_NEXT  (_BV(TWINT) | _BV(TWEN) | _BV(TWIE))   // send TWDR / receive + NACK
#define TWCR_ACK   (TWCR_NEXT | _BV(TWEA))                // receive + ACK ("send more")
#define TWCR_START (TWCR_NEXT | _BV(TWSTA))               // (repeated) START
#define TWCR_STOP  (_BV(TWINT) | _BV(TWEN) | _BV(TWSTO))  // STOP, no more interrupts


void I2cDrv::begin(uint32_t clockHz)
{
    instance = this;

    // Pull-ups on SDA/SCL (the DS3231 module also has its own).
    digitalWrite(SDA, HIGH);
    digitalWrite(SCL, HIGH);

    // SCL = F_CPU / (16 + 2 * TWBR). 16 MHz, 100 kHz -> TWBR = 72.
    TWSR = 0;
    TWBR = ((F_CPU / clockHz) - 16) / 2;
    TWCR = _BV(TWEN);
}

bool I2cDrv::read(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t size)
{
    // Only 7-bit addresses: 0x00..0x7F.
    if (deviceAddress > 0x7F || data == nullptr || size == 0)
        return false;

    this->address = deviceAddress;
    this->reg     = reg;
    this->isRead  = true;
    this->rxData  = data;
    this->txData  = nullptr;
    this->size    = size;
    return startAndWait();
}

bool I2cDrv::write(uint8_t deviceAddress, uint8_t reg, const uint8_t* data, uint8_t size)
{
    if (deviceAddress > 0x7F || (data == nullptr && size > 0))
        return false;

    this->address = deviceAddress;
    this->reg     = reg;
    this->isRead  = false;
    this->rxData  = nullptr;
    this->txData  = data;
    this->size    = size;
    return startAndWait();
}

bool I2cDrv::startAndWait()
{
    const uint32_t startMs = millis();

    // The STOP of the previous transfer may still be on its way.
    while (TWCR & _BV(TWSTO))
    {
        if (millis() - startMs > TIMEOUT_MS)
            return false;
    }

    // 1. START. From here on the interrupt does the work.
    index  = 0;
    result = BUSY;
    asm volatile("" ::: "memory");  // all fields above must be in memory before the ISR runs
    TWCR = TWCR_START;

    // 2. WAIT until the interrupt reports the end - or give up.
    while (result == BUSY)
    {
        if (millis() - startMs > TIMEOUT_MS)
        {
            // Bus stuck. Switch TWI off and on: resets it, no more interrupts.
            TWCR = 0;
            TWCR = _BV(TWEN);
            return false;
        }
    }
    return result == DONE;
}

void I2cDrv::onInterrupt()
{
    switch (TW_STATUS)
    {
    // ---- Address + register: always a WRITE first ----
    case TW_START:                      // START is on the bus
        TWDR = (address << 1) | TW_WRITE; // 7-bit address + R/W bit = 8 bits on the wire
        TWCR = TWCR_NEXT;
        break;

    case TW_MT_SLA_ACK:                 // device answered its address
        TWDR = reg;
        TWCR = TWCR_NEXT;
        break;

    case TW_MT_DATA_ACK:                // device accepted a byte
        if (isRead)
        {
            TWCR = TWCR_START;          // register sent: REPEATED START, turn to reading
        }
        else if (index < size)
        {
            TWDR = txData[index++];     // next data byte
            TWCR = TWCR_NEXT;
        }
        else
        {
            finish(DONE);               // all written
        }
        break;

    // ---- Reading ----
    case TW_REP_START:
        TWDR = (address << 1) | TW_READ;
        TWCR = TWCR_NEXT;
        break;

    case TW_MR_SLA_ACK:
        // ACK every byte except the last: the NACK says "enough" to the device.
        TWCR = (size > 1) ? TWCR_ACK : TWCR_NEXT;
        break;

    case TW_MR_DATA_ACK:
        rxData[index++] = TWDR;
        TWCR = (index + 1 < size) ? TWCR_ACK : TWCR_NEXT;
        break;

    case TW_MR_DATA_NACK:               // the last byte
        rxData[index++] = TWDR;
        finish(DONE);
        break;

    // ---- Everything else: no answer (NACK), bus error, ... ----
    default:
        finish(FAILED);
        break;
    }
}

void I2cDrv::finish(Result r)
{
    TWCR = TWCR_STOP;
    result = r;       // this is the "event" that read()/write() are waiting for
}
