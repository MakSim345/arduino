#ifndef EDU67_CLOCKIF_H
#define EDU67_CLOCKIF_H

#include "Platform.h"

/**
 * Source of time for timeouts.
 * The drivers never call millis() directly, so a unit test can give them a fake clock.
 */
class ClockIf
{
public:
    virtual ~ClockIf() {}

    /** Milliseconds since start. Wraps around after ~49 days: always compare (now - start). */
    virtual uint32_t nowMs() = 0;
};

#if defined(ARDUINO)
class ArduinoClock : public ClockIf
{
public:
    uint32_t nowMs() override { return millis(); }
};
#endif

#endif // EDU67_CLOCKIF_H
