#ifndef EDU67_RECURSIVELOCK_H
#define EDU67_RECURSIVELOCK_H

#include "Platform.h"

/**
 * The bus "mutex".
 *
 * With an RTOS (FreeRTOS on ESP32, Arduino_FreeRTOS on AVR) this would be a
 * recursive mutex: xSemaphoreTakeRecursive() / xSemaphoreGiveRecursive().
 *
 * A bare-metal sketch has only one thread, so nobody can steal the bus between
 * lock() and unlock(). The counter still documents and checks the contract:
 * - "recursive" = the same owner may lock again. A sensor driver locks the bus
 *   for a read-modify-write sequence, and each read()/write() inside locks it too.
 * - unlock() without lock() is a bug and stops the program.
 */
class RecursiveLock
{
public:
    RecursiveLock() : depthM(0) {}

    void lock()
    {
        EDU_ASSERT(depthM < 255);
        ++depthM;
    }

    void unlock()
    {
        EDU_ASSERT(depthM > 0);
        --depthM;
    }

    bool isLocked() const { return depthM > 0; }

private:
    uint8_t depthM;
};

#endif // EDU67_RECURSIVELOCK_H
