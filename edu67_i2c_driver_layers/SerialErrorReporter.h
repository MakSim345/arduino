#ifndef EDU67_SERIALERRORREPORTER_H
#define EDU67_SERIALERRORREPORTER_H

#include "ErrorReporterIf.h"

#if defined(ARDUINO)

/** Prints errors to the Serial Monitor. */
class SerialErrorReporter : public ErrorReporterIf
{
public:
    SerialErrorReporter() : fatalCountM(0) {}

    void reportFatal(Subsystem subsystemP, HwError errorP) override
    {
        ++fatalCountM;
        Serial.print(F("FATAL: subsystem="));
        Serial.print(static_cast<uint8_t>(subsystemP));
        Serial.print(F(" error="));
        Serial.println(static_cast<uint8_t>(errorP));
    }

    void logError(EduText messageP, uint16_t detailP) override
    {
        Serial.print(F("ERROR: "));
        Serial.print(messageP);
        Serial.print(F(" (0x"));
        Serial.print(detailP, HEX);
        Serial.println(')');
    }

    uint8_t fatalCount() const { return fatalCountM; }

private:
    uint8_t fatalCountM;
};

#endif

#endif // EDU67_SERIALERRORREPORTER_H
