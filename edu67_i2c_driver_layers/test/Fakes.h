#ifndef EDU67_TEST_FAKES_H
#define EDU67_TEST_FAKES_H

/*
 * Fakes: small working imitations of the real things.
 *
 * A mock (as in GoogleMock) is told in each test exactly which calls to
 * expect. A fake just behaves like the real thing, and the test looks at
 * the result. For hardware, fakes often give shorter and less brittle tests.
 */

#include "ClockIf.h"
#include "ErrorReporterIf.h"
#include "I2cDrvIf.h"
#include "TwiHalIf.h"
#include "WireIf.h"

#include <cstdio>
#include <deque>
#include <map>
#include <string>
#include <vector>

/** "68 0E" style hex text of bytes. */
inline std::string hexBytes(const std::vector<uint8_t>& bytesP)
{
    std::string text;
    char buffer[4];
    for (size_t i = 0; i < bytesP.size(); ++i)
    {
        std::snprintf(buffer, sizeof(buffer), i ? " %02X" : "%02X", bytesP[i]);
        text += buffer;
    }
    return text;
}

// -----------------------------------------------------------------------------

/** Clock that moves 1 ms forward every time somebody looks at it. */
class FakeClock : public ClockIf
{
public:
    uint32_t nowMs() override { return nowMsM++; }
    uint32_t nowMsM = 0;
};

// -----------------------------------------------------------------------------

/** Remembers everything that was reported. */
class FakeReporter : public ErrorReporterIf
{
public:
    void reportFatal(Subsystem subsystemP, HwError errorP) override
    {
        fatals.push_back(std::make_pair(subsystemP, errorP));
    }

    void logError(EduText messageP, uint16_t detailP) override
    {
        logs.push_back(std::string(messageP) + " " + std::to_string(detailP));
    }

    std::vector<std::pair<Subsystem, HwError>> fatals;
    std::vector<std::string> logs;
};

// -----------------------------------------------------------------------------

/**
 * Fake Wire library. Records transactions as text, e.g.
 *   "W 68: 11 Sr"  = write to 0x68 the byte 0x11, then a REPEATED START
 *   "R 68: 2 P"    = read 2 bytes from 0x68, then STOP
 */
class FakeWire : public WireIf
{
public:
    void beginTransmission(uint8_t addressP) override
    {
        addressM = addressP;
        txM.clear();
    }

    size_t write(uint8_t valueP) override
    {
        if (txM.size() >= bufferSizeM)
        {
            return 0; // like the real Wire: the byte is dropped
        }
        txM.push_back(valueP);
        return 1;
    }

    uint8_t endTransmission(bool sendStopP) override
    {
        char head[16];
        std::snprintf(head, sizeof(head), "W %02X: ", addressM);
        log.push_back(head + hexBytes(txM) + (sendStopP ? " P" : " Sr"));
        return nextResult(endResults, WIRE_OK);
    }

    uint8_t requestFrom(uint8_t addressP, uint8_t countP, bool sendStopP) override
    {
        char text[32];
        std::snprintf(text, sizeof(text), "R %02X: %u%s", addressP, countP, sendStopP ? " P" : "");
        log.push_back(text);

        const uint8_t received = static_cast<uint8_t>(nextResult(requestResults, countP));
        rxM.clear();
        for (uint8_t i = 0; (i < received) && !rxData.empty(); ++i)
        {
            rxM.push_back(rxData.front());
            rxData.pop_front();
        }
        return received;
    }

    int read() override
    {
        if (rxM.empty())
        {
            return -1;
        }
        const int value = rxM.front();
        rxM.pop_front();
        return value;
    }

    size_t bufferSize() const override { return bufferSizeM; }

    bool takeTimeoutFlag() override
    {
        const bool flag = timeoutFlag;
        timeoutFlag = false;
        return flag;
    }

    // --- Test controls ---
    std::vector<std::string> log;
    std::deque<uint8_t> endResults;     ///< next endTransmission() results (default WIRE_OK)
    std::deque<int> requestResults;     ///< next requestFrom() results (default: all bytes)
    std::deque<uint8_t> rxData;         ///< bytes the "device" sends
    bool timeoutFlag = false;
    size_t bufferSizeM = 32;

private:
    template <typename T>
    static T nextResult(std::deque<int>& rQueueP, T defaultP)
    {
        if (rQueueP.empty()) { return defaultP; }
        const T value = static_cast<T>(rQueueP.front());
        rQueueP.pop_front();
        return value;
    }
    static uint8_t nextResult(std::deque<uint8_t>& rQueueP, uint8_t defaultP)
    {
        if (rQueueP.empty()) { return defaultP; }
        const uint8_t value = rQueueP.front();
        rQueueP.pop_front();
        return value;
    }

    uint8_t addressM = 0;
    std::vector<uint8_t> txM;
    std::deque<uint8_t> rxM;
};

// -----------------------------------------------------------------------------

/**
 * Fake interrupt-driven HAL. The test decides what the "interrupt" does:
 * finish at once (inside memReadIt), later (test calls fire...()), or never.
 */
class FakeTwiHal : public TwiHalIf
{
public:
    enum class Reaction { NEVER, COMPLETE, ERROR, WRONG_EVENT };

    bool memReadIt(uint8_t address8bitP, uint16_t registerP, uint8_t registerSizeP,
                   uint8_t* pDataP, uint16_t sizeP) override
    {
        lastAddress8bit = address8bitP;
        lastRegister = registerP;
        lastRegisterSize = registerSizeP;
        lastSize = sizeP;
        ++readStarts;
        if (!startSucceeds)
        {
            error = I2cResult::BUSY;
            return false;
        }
        for (uint16_t i = 0; (i < sizeP) && (i < rxData.size()); ++i)
        {
            pDataP[i] = rxData[i];
        }
        react(true);
        return true;
    }

    bool memWriteIt(uint8_t address8bitP, uint16_t registerP, uint8_t registerSizeP,
                    const uint8_t* pDataP, uint16_t sizeP) override
    {
        lastAddress8bit = address8bitP;
        lastRegister = registerP;
        lastRegisterSize = registerSizeP;
        lastSize = sizeP;
        written.assign(pDataP, pDataP + sizeP);
        ++writeStarts;
        if (!startSucceeds)
        {
            error = I2cResult::BUSY;
            return false;
        }
        react(false);
        return true;
    }

    void abort() override { ++aborts; }

    void setCallbacks(CallbacksIf& rCallbacksP) override { pCallbacks = &rCallbacksP; }
    void resetCallbacks(CallbacksIf& rCallbacksP) override
    {
        if (pCallbacks == &rCallbacksP) { pCallbacks = nullptr; }
    }

    I2cResult getError() const override { return error; }

    // "Interrupts" the test can fire by hand.
    void fireRxComplete() { pCallbacks->memRxCpltCallback(); }
    void fireTxComplete() { pCallbacks->memTxCpltCallback(); }
    void fireError(I2cResult errorP) { error = errorP; pCallbacks->errorCallback(); }

    // --- Test controls and observations ---
    Reaction reaction = Reaction::COMPLETE;
    I2cResult errorToReport = I2cResult::NACK_ADDRESS;
    bool startSucceeds = true;
    std::vector<uint8_t> rxData;
    std::vector<uint8_t> written;
    CallbacksIf* pCallbacks = nullptr;
    I2cResult error = I2cResult::OK;
    uint8_t lastAddress8bit = 0;
    uint16_t lastRegister = 0;
    uint8_t lastRegisterSize = 0;
    uint16_t lastSize = 0;
    int readStarts = 0;
    int writeStarts = 0;
    int aborts = 0;

private:
    void react(bool isReadP)
    {
        switch (reaction)
        {
        case Reaction::NEVER:    break;
        case Reaction::COMPLETE: isReadP ? fireRxComplete() : fireTxComplete(); break;
        case Reaction::ERROR:    fireError(errorToReport); break;
        case Reaction::WRONG_EVENT: isReadP ? fireTxComplete() : fireRxComplete(); break;
        }
    }
};

// -----------------------------------------------------------------------------

/**
 * Fake I2C bus with simulated chips on it: each device is just an array of
 * registers with an auto-incrementing address, like most real I2C chips.
 *
 * Checks the locking contract: records the lock depth seen by every transfer.
 */
class FakeI2cBus : public I2cDrvIf
{
public:
    struct Device
    {
        std::vector<uint8_t> memory;
        uint16_t pageSize = 0;   ///< >0: writes wrap inside a page, like a real EEPROM
        int busyTransfers = 0;   ///< NACK this many transfers after every write
        int busyLeft = 0;
    };

    void addDevice(uint8_t addressP, size_t sizeP, uint16_t pageSizeP = 0, int busyTransfersP = 0)
    {
        Device device;
        device.memory.assign(sizeP, 0);
        device.pageSize = pageSizeP;
        device.busyTransfers = busyTransfersP;
        devices[addressP] = device;
    }

    void lock() override { ++lockDepth; }
    void unlock() override { --lockDepth; }

    bool read(uint16_t deviceP, uint16_t registerP, uint16_t registerSizeP,
              uint8_t* pDataP, size_t sizeP) override
    {
        record("R", deviceP, registerP, registerSizeP, sizeP);
        Device* pDevice = answer(deviceP);
        if (!pDevice || (registerP + sizeP > pDevice->memory.size()))
        {
            return false;
        }
        for (size_t i = 0; i < sizeP; ++i)
        {
            pDataP[i] = pDevice->memory[registerP + i];
        }
        return true;
    }

    bool write(uint16_t deviceP, uint16_t registerP, uint16_t registerSizeP,
               const uint8_t* pDataP, size_t sizeP) override
    {
        record("W", deviceP, registerP, registerSizeP, sizeP);
        Device* pDevice = answer(deviceP);
        if (!pDevice || (registerP >= pDevice->memory.size()))
        {
            return false;
        }
        for (size_t i = 0; i < sizeP; ++i)
        {
            size_t target = registerP + i;
            if (pDevice->pageSize)
            {
                // Real EEPROM: the address counter wraps at the end of the page.
                const size_t pageStart = registerP - (registerP % pDevice->pageSize);
                target = pageStart + ((registerP + i) - pageStart) % pDevice->pageSize;
            }
            pDevice->memory[target] = pDataP[i];
        }
        pDevice->busyLeft = pDevice->busyTransfers;
        return true;
    }

    std::map<uint8_t, Device> devices;
    std::vector<std::string> log;       ///< e.g. "R 68 @11 x2"
    std::vector<int> lockDepthSeen;     ///< lock depth during each transfer
    int lockDepth = 0;
    int failAfter = -1;                 ///< >=0: this many transfers work, then the bus dies

private:
    Device* answer(uint16_t deviceP)
    {
        if (failAfter == 0)
        {
            return nullptr;
        }
        if (failAfter > 0)
        {
            --failAfter;
        }
        auto it = devices.find(static_cast<uint8_t>(deviceP));
        if (it == devices.end())
        {
            return nullptr;
        }
        if (it->second.busyLeft > 0)
        {
            --it->second.busyLeft; // busy: NACK
            return nullptr;
        }
        return &it->second;
    }

    void record(const char* opP, uint16_t deviceP, uint16_t registerP, uint16_t registerSizeP, size_t sizeP)
    {
        char text[40];
        std::snprintf(text, sizeof(text), registerSizeP == 2 ? "%s %02X @%04X x%u" : "%s %02X @%02X x%u",
                      opP, deviceP, registerP, static_cast<unsigned>(sizeP));
        log.push_back(text);
        lockDepthSeen.push_back(lockDepth);
    }
};

#endif // EDU67_TEST_FAKES_H
