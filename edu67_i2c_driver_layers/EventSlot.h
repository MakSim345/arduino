#ifndef EDU67_EVENTSLOT_H
#define EDU67_EVENTSLOT_H

#include "ClockIf.h"

/**
 * A queue of length 1 between an interrupt and the main code.
 *
 * With an RTOS this would be xQueueSendFromISR() / xQueueReceive(timeout),
 * and the waiting task would sleep. Without an RTOS we busy-wait on a
 * volatile flag - the idea is the same.
 *
 * Length 1 is enough: only one transfer runs at a time, and it ends with
 * exactly one event.
 *
 * Why "volatile": the flag is changed by the ISR, behind the compiler's back.
 * Without volatile the compiler may read it once and loop forever.
 */
class EventSlot
{
public:
    EventSlot() : fullM(false), eventM(0), detailM(0) {}

    /** ISR side. @return false if the slot is still full (event lost). */
    bool trySendFromIsr(uint8_t eventP, uint8_t detailP)
    {
        if (fullM)
        {
            return false;
        }
        eventM  = eventP;
        detailM = detailP;
        fullM   = true; // Last: the main code must never see "full" before the data.
        return true;
    }

    /** Main code side. Waits up to timeoutMsP. @return false on timeout. */
    bool tryReceive(uint8_t& rEventP, uint8_t& rDetailP, ClockIf& rClockP, uint16_t timeoutMsP)
    {
        const uint32_t startMs = rClockP.nowMs();
        while (!fullM)
        {
            // Unsigned subtraction: correct even when millis() wraps around.
            if (static_cast<uint32_t>(rClockP.nowMs() - startMs) >= timeoutMsP)
            {
                return false;
            }
        }
        rEventP  = eventM;
        rDetailP = detailM;
        fullM    = false;
        return true;
    }

    /** Throws away an old event (e.g. one that arrived after a timeout). */
    void clear() { fullM = false; } // One byte: the write is atomic on AVR.

private:
    volatile bool fullM;
    volatile uint8_t eventM;
    volatile uint8_t detailM;
};

#endif // EDU67_EVENTSLOT_H
