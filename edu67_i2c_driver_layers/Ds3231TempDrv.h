#ifndef EDU67_DS3231TEMPDRV_H
#define EDU67_DS3231TEMPDRV_H

#include "FatalErrorLatch.h"
#include "I2cDrvIf.h"

/** DS3231 I2C address (fixed, cannot be changed). */
constexpr uint8_t DS3231_ADDRESS = 0x68;

/** DS3231 registers used by this driver (datasheet, "Timekeeping Registers"). */
constexpr uint8_t DS3231_REG_CONTROL  = 0x0E;
constexpr uint8_t DS3231_REG_STATUS   = 0x0F;
constexpr uint8_t DS3231_REG_TEMP_MSB = 0x11;
constexpr uint8_t DS3231_REG_TEMP_LSB = 0x12;

constexpr uint8_t DS3231_CONTROL_CONV = 0x20; ///< Write 1: start a temperature conversion now
constexpr uint8_t DS3231_STATUS_BSY   = 0x04; ///< 1: a conversion is running

/**
 * Temperature sensor of the DS3231 real-time clock.
 *
 * The DS3231 measures temperature to correct its crystal. It converts on its
 * own only every 64 seconds, so after each reading we ask for a new
 * conversion: the NEXT reading is then fresh (the value is "one call behind").
 *
 * Temperature unit: 1/10000 degree Celsius, in an int32_t.
 * 25.5 C = 255000. Integers only: no float on an 8-bit CPU without an FPU.
 */
class Ds3231TempDrv
{
public:
    Ds3231TempDrv(I2cDrvIf& rBusP, uint8_t addressP = DS3231_ADDRESS);

    Ds3231TempDrv(const Ds3231TempDrv&) = delete;
    Ds3231TempDrv& operator=(const Ds3231TempDrv&) = delete;

    void setErrorReporter(ErrorReporterIf& rReporterP) { latchM.setReporter(rReporterP); }

    /** @return true if the driver has given up (the chip did not answer). */
    bool hasFatalFailure() const { return latchM.isSet(); }

    /**
     * Checks that the chip answers.
     * Writes nothing: the same chip keeps the time, and its alarms and
     * square-wave output may be used by somebody else.
     */
    bool init();

    /** @param[out] rTemperatureE4P temperature in 1/10000 C. */
    bool getTemperature(int32_t& rTemperatureE4P);

    /** init() + one reading + range check against the datasheet (-40..+85 C). */
    bool selfTest();

private:
    /** Logs, sets the fatal failure, returns false. */
    bool fail(EduText messageP);

    I2cDrvIf& rBusM;
    const uint8_t addressM;
    FatalErrorLatch latchM;
};

#endif // EDU67_DS3231TEMPDRV_H
