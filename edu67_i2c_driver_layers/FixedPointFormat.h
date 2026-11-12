#ifndef EDU67_FIXEDPOINTFORMAT_H
#define EDU67_FIXEDPOINTFORMAT_H

#include "Platform.h"

/** Enough for "-214748.3648" plus the terminating zero. */
constexpr size_t FIXED_E4_TEXT_SIZE = 13;

/**
 * Formats a value in 1/10000 units: 255000 -> "25.5000", -2500 -> "-0.2500".
 *
 * The naive way, print(v / 10000) '.' print(v % 10000), gives "0.-2500"
 * for -2500 and "25.0" for 250000 - both wrong. Work with the magnitude,
 * and print the sign yourself.
 */
inline void formatFixedE4(int32_t valueE4P, char* pTextP, size_t textSizeP)
{
    EDU_ASSERT(pTextP != nullptr);
    EDU_ASSERT(textSizeP >= FIXED_E4_TEXT_SIZE);

    // Negate as unsigned: -(-2147483648) does not fit in int32_t.
    const bool isNegative = valueE4P < 0;
    const uint32_t magnitude = isNegative
        ? (0u - static_cast<uint32_t>(valueE4P))
        : static_cast<uint32_t>(valueE4P);

    uint32_t whole = magnitude / 10000u;
    uint32_t fraction = magnitude % 10000u;

    // Digits come out backwards: build the text from the end.
    char buffer[FIXED_E4_TEXT_SIZE];
    size_t pos = sizeof(buffer);
    buffer[--pos] = '\0';
    for (int i = 0; i < 4; ++i)
    {
        buffer[--pos] = static_cast<char>('0' + (fraction % 10u));
        fraction /= 10u;
    }
    buffer[--pos] = '.';
    do
    {
        buffer[--pos] = static_cast<char>('0' + (whole % 10u));
        whole /= 10u;
    } while (whole > 0);
    if (isNegative)
    {
        buffer[--pos] = '-';
    }

    size_t out = 0;
    while (pos < sizeof(buffer))
    {
        pTextP[out++] = buffer[pos++];
    }
}

#endif // EDU67_FIXEDPOINTFORMAT_H
