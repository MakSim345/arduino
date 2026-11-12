#include "WireAdapter.h"

// Wire.h is included ONLY for the Wire backend. The Arduino builder follows
// #include lines through the preprocessor, so with I2C_BACKEND_IT the Wire
// library is not linked at all - and its ISR(TWI_vect) does not collide with ours.
#if defined(ARDUINO) && defined(I2C_BACKEND_WIRE)

#include <Wire.h>

void WireAdapter::begin(uint32_t clockHzP, uint32_t timeoutUsP, bool internalPullupsP)
{
    Wire.begin();
    if (!internalPullupsP)
    {
        // The pins stay under TWI control; LOW only switches the pull-up off.
        digitalWrite(SDA, LOW);
        digitalWrite(SCL, LOW);
    }
    Wire.setClock(clockHzP);

#if defined(WIRE_HAS_TIMEOUT)
    // true = also reset the TWI hardware after a timeout.
    Wire.setWireTimeout(timeoutUsP, true);
#else
    (void)timeoutUsP; // Old core: a stuck bus hangs forever. Update the board package!
#endif
}

void WireAdapter::beginTransmission(uint8_t addressP)
{
    Wire.beginTransmission(addressP);
}

size_t WireAdapter::write(uint8_t valueP)
{
    return Wire.write(valueP);
}

uint8_t WireAdapter::endTransmission(bool sendStopP)
{
    return Wire.endTransmission(static_cast<uint8_t>(sendStopP));
}

uint8_t WireAdapter::requestFrom(uint8_t addressP, uint8_t countP, bool sendStopP)
{
    return Wire.requestFrom(addressP, countP, static_cast<uint8_t>(sendStopP));
}

int WireAdapter::read()
{
    return Wire.read();
}

size_t WireAdapter::bufferSize() const
{
#if defined(BUFFER_LENGTH)
    return BUFFER_LENGTH;
#else
    return 32;
#endif
}

bool WireAdapter::takeTimeoutFlag()
{
#if defined(WIRE_HAS_TIMEOUT)
    const bool timedOut = Wire.getWireTimeoutFlag();
    Wire.clearWireTimeoutFlag();
    return timedOut;
#else
    return false;
#endif
}

#endif
