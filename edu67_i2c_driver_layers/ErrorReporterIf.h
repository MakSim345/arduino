#ifndef EDU67_ERRORREPORTERIF_H
#define EDU67_ERRORREPORTERIF_H

#include "Platform.h"

/** Which part of the system failed. */
enum class Subsystem : uint8_t
{
    I2C_BUS     = 1,
    PERIPHERALS = 2
};

/** What failed. */
enum class HwError : uint8_t
{
    I2C_FAILURE     = 1,
    DS3231_FAILURE  = 2,
    AT24C32_FAILURE = 3
};

/**
 * Where drivers send their problems.
 *
 * Drivers do not print to Serial themselves: they do not know if the sketch
 * wants Serial, an LED, a display or nothing at all. The sketch decides by
 * giving them an implementation of this interface (see SerialErrorReporter.h).
 */
class ErrorReporterIf
{
public:
    virtual ~ErrorReporterIf() {}

    /** Hardware is broken; the application should go to a safe state. */
    virtual void reportFatal(Subsystem subsystemP, HwError errorP) = 0;

    /** Diagnostic message. detailP is an extra number (error code, register, ...). */
    virtual void logError(EduText messageP, uint16_t detailP) = 0;
};

#endif // EDU67_ERRORREPORTERIF_H
