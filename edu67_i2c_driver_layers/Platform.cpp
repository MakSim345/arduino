#include "Platform.h"

#if defined(ARDUINO)

void eduAssertFailed(EduText file, int line)
{
    Serial.print(F("ASSERT FAILED: "));
    Serial.print(file);
    Serial.print(':');
    Serial.println(line);
    Serial.flush();

    // Stop here: a fast blinking LED means "the program found a bug in itself".
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;)
    {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        delay(100);
    }
}

#endif
