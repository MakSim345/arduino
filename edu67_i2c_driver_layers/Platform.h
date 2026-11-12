#ifndef EDU67_PLATFORM_H
#define EDU67_PLATFORM_H

/*
 * The small set of things that differ between the Arduino board and the PC
 * where the unit tests run. Everything else in the project is plain C++11.
 */

#include <stddef.h>
#include <stdint.h>

#if defined(ARDUINO)

#include <Arduino.h>

// Text kept in flash (F() macro) instead of the 2 KB of RAM of the ATmega328P.
typedef const __FlashStringHelper* EduText;
#define EDU_TEXT(s) F(s)

// Prints file:line to Serial and blinks the LED forever.
void eduAssertFailed(EduText file, int line);
#define EDU_ASSERT(expr) do { if (!(expr)) { eduAssertFailed(F(__FILE__), __LINE__); } } while (0)

#if defined(ARDUINO_ARCH_AVR)
/** Disables interrupts for its lifetime and restores the previous state (nesting-safe). */
class CriticalSection
{
public:
    CriticalSection() : sregM(SREG) { cli(); }
    ~CriticalSection() { SREG = sregM; }
    CriticalSection(const CriticalSection&) = delete;
    CriticalSection& operator=(const CriticalSection&) = delete;
private:
    uint8_t sregM;
};

inline bool eduInterruptsEnabled() { return (SREG & _BV(SREG_I)) != 0; }
#else
/** Non-AVR boards: simple version, NOT nesting-safe. */
class CriticalSection
{
public:
    CriticalSection() { noInterrupts(); }
    ~CriticalSection() { interrupts(); }
    CriticalSection(const CriticalSection&) = delete;
    CriticalSection& operator=(const CriticalSection&) = delete;
};

inline bool eduInterruptsEnabled() { return true; }
#endif

#else // PC unit tests

#include <stdexcept>

typedef const char* EduText;
#define EDU_TEXT(s) (s)

/** On the PC a failed assert throws, so a test can check that it happened. */
class EduAssertFailure : public std::logic_error
{
public:
    explicit EduAssertFailure(const char* expr) : std::logic_error(expr) {}
};
#define EDU_ASSERT(expr) do { if (!(expr)) { throw EduAssertFailure(#expr); } } while (0)

/** There are no interrupts in the unit tests. */
class CriticalSection
{
public:
    CriticalSection() {}
    CriticalSection(const CriticalSection&) = delete;
    CriticalSection& operator=(const CriticalSection&) = delete;
};

inline bool eduInterruptsEnabled() { return true; }

#endif

#endif // EDU67_PLATFORM_H
