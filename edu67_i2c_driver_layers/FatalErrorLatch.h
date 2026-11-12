#ifndef EDU67_FATALERRORLATCH_H
#define EDU67_FATALERRORLATCH_H

#include "ErrorReporterIf.h"

/**
 * Remembers that a driver has failed, and reports it exactly once.
 *
 * Two patterns worth copying:
 * 1. Report once. A broken sensor read every 100 ms must not flood the log
 *    with the same error 10 times per second.
 * 2. Late reporter. Drivers are often created (and fail!) before the error
 *    handling is ready. When the reporter is connected later, an earlier
 *    failure is replayed to it, so it is never lost.
 */
class FatalErrorLatch
{
public:
    FatalErrorLatch(Subsystem subsystemP, HwError errorP) :
        subsystemM(subsystemP),
        errorM(errorP),
        pReporterM(nullptr),
        isSetM(false)
    {
    }

    void setReporter(ErrorReporterIf& rReporterP)
    {
        if (pReporterM == &rReporterP)
        {
            return;
        }
        pReporterM = &rReporterP;

        // Replay a failure that happened before we had someone to tell.
        if (isSetM)
        {
            pReporterM->reportFatal(subsystemM, errorM);
        }
    }

    void set()
    {
        if (isSetM)
        {
            return;
        }
        isSetM = true;

        if (pReporterM)
        {
            pReporterM->reportFatal(subsystemM, errorM);
        }
    }

    bool isSet() const { return isSetM; }

    /** Logs through the reporter, if there is one. */
    void log(EduText messageP, uint16_t detailP) const
    {
        if (pReporterM)
        {
            pReporterM->logError(messageP, detailP);
        }
    }

private:
    const Subsystem subsystemM;
    const HwError errorM;
    ErrorReporterIf* pReporterM;
    bool isSetM;
};

#endif // EDU67_FATALERRORLATCH_H
