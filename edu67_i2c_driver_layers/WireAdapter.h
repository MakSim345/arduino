#ifndef EDU67_WIREADAPTER_H
#define EDU67_WIREADAPTER_H

#include "Config.h"
#include "WireIf.h"

#if defined(ARDUINO) && defined(I2C_BACKEND_WIRE)

/**
 * The real WireIf: forwards every call to the global Wire object.
 * No logic here on purpose - everything worth testing lives in I2cWireDrv.
 */
class WireAdapter : public WireIf
{
public:
    /**
     * @param[in] clockHzP          100000 (standard) or 400000 (fast mode).
     * @param[in] timeoutUsP        Give up if the bus is stuck for this long.
     * @param[in] internalPullupsP  Wire.begin() turns on the ~40 kOhm pull-ups of the AVR.
     *                              Modules usually have their own 4.7 kOhm ones, and a
     *                              3.3 V sensor must NOT be pulled up to 5 V: then pass false.
     */
    void begin(uint32_t clockHzP, uint32_t timeoutUsP, bool internalPullupsP);

    void beginTransmission(uint8_t addressP) override;
    size_t write(uint8_t valueP) override;
    uint8_t endTransmission(bool sendStopP) override;
    uint8_t requestFrom(uint8_t addressP, uint8_t countP, bool sendStopP) override;
    int read() override;
    size_t bufferSize() const override;
    bool takeTimeoutFlag() override;
};

#endif

#endif // EDU67_WIREADAPTER_H
