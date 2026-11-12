#ifndef EDU67_WIREIF_H
#define EDU67_WIREIF_H

#include "Platform.h"

/** Return codes of Wire.endTransmission(). */
enum WireStatus : uint8_t
{
    WIRE_OK            = 0,
    WIRE_DATA_TOO_LONG = 1,
    WIRE_NACK_ADDRESS  = 2,
    WIRE_NACK_DATA     = 3,
    WIRE_OTHER_ERROR   = 4,
    WIRE_TIMEOUT       = 5  ///< Since AVR core 1.8.3, when Wire.setWireTimeout() is used
};

/**
 * The part of the Arduino Wire library that the driver uses.
 *
 * Why wrap Wire? The global "Wire" object talks to real hardware, so code
 * that calls it directly can only be tested on a board. Code that talks to
 * this interface can be tested on a PC with a fake (see test/FakeWire.h).
 */
class WireIf
{
public:
    virtual ~WireIf() {}

    virtual void beginTransmission(uint8_t addressP) = 0;

    /** @return 1 if the byte was queued, 0 if the Wire buffer is full. */
    virtual size_t write(uint8_t valueP) = 0;

    /**
     * Sends the queued bytes.
     * @param[in] sendStopP false = keep the bus for a REPEATED START (register read).
     * @return a WireStatus value.
     */
    virtual uint8_t endTransmission(bool sendStopP) = 0;

    /** @return how many bytes were received (0 on NACK or timeout). */
    virtual uint8_t requestFrom(uint8_t addressP, uint8_t countP, bool sendStopP) = 0;

    /** @return next received byte, or -1 if none is left. */
    virtual int read() = 0;

    /** @return size of the Wire buffer (32 bytes on AVR). */
    virtual size_t bufferSize() const = 0;

    /** @return true if a timeout happened since the last call. Clears the flag. */
    virtual bool takeTimeoutFlag() = 0;
};

#endif // EDU67_WIREIF_H
