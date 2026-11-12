#ifndef EDU67_I2CDRVBASE_H
#define EDU67_I2CDRVBASE_H

#include "FatalErrorLatch.h"
#include "I2cDrvIf.h"
#include "RecursiveLock.h"

/**
 * What every bus driver has in common: the lock, argument checks, and
 * the error policy. Only "how bytes get on the wire" differs.
 *
 * Error policy:
 * - NACK is normal life: a device is not there, or is busy. The caller
 *   decides what it means, so we only return false.
 * - A stuck or broken bus (timeout, bus error, cannot start) is a fatal
 *   hardware problem: logged and reported once.
 */
class I2cDrvBase : public I2cDrvIf
{
public:
    /** @see I2cDrvIf::lock */
    void lock() override { lockM.lock(); }

    /** @see I2cDrvIf::unlock */
    void unlock() override { lockM.unlock(); }

    /** @param[in] rReporterP Where to report errors. May be connected late. */
    void setErrorReporter(ErrorReporterIf& rReporterP) { latchM.setReporter(rReporterP); }

    /** @return true if a fatal bus failure has happened. The driver still tries to work. */
    bool hasFatalFailure() const { return latchM.isSet(); }

    /** @return result of the last read()/write(). */
    I2cResult lastResult() const { return lastResultM; }

protected:
    I2cDrvBase();
    ~I2cDrvBase() override {}

    I2cDrvBase(const I2cDrvBase&) = delete;
    I2cDrvBase& operator=(const I2cDrvBase&) = delete;

    /** Stops the program if the arguments are a programming error. */
    static void checkRequest(
        uint16_t deviceAddressP,
        uint16_t registerAddressSizeP,
        const void* pDataP);

    /** Records success. @return true */
    bool succeed();

    /** Records and classifies a failure. @return false */
    bool fail(I2cResult resultP);

    /** Logs through the error reporter, if connected. */
    void log(EduText messageP, uint16_t detailP) const { latchM.log(messageP, detailP); }

private:
    RecursiveLock lockM;
    FatalErrorLatch latchM;
    I2cResult lastResultM;
};

#endif // EDU67_I2CDRVBASE_H
