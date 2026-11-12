#include "TinyTest.h"
#include "Fakes.h"

#include "I2cWireDrv.h"
#include "LockGuard.h"

TEST(I2cWireDrv, readSendsRegisterThenRepeatedStartThenReads)
{
    FakeWire wire;
    I2cWireDrv bus(wire);
    wire.rxData = {0x19, 0x40};

    uint8_t data[2] = {0, 0};
    EXPECT_TRUE(bus.read(0x68, 0x11, 1, data, 2));

    // No STOP after the register address ("Sr"), STOP after the data.
    EXPECT_EQ(wire.log.size(), 2u);
    EXPECT_EQ(wire.log[0], "W 68: 11 Sr");
    EXPECT_EQ(wire.log[1], "R 68: 2 P");
    EXPECT_EQ(data[0], 0x19);
    EXPECT_EQ(data[1], 0x40);
    EXPECT_EQ(bus.lastResult(), I2cResult::OK);
}

TEST(I2cWireDrv, twoByteRegisterAddressIsSentMsbFirst)
{
    FakeWire wire;
    I2cWireDrv bus(wire);
    wire.rxData = {0xAB};

    uint8_t data = 0;
    EXPECT_TRUE(bus.read(0x57, 0x0F1E, 2, &data, 1));
    EXPECT_EQ(wire.log[0], "W 57: 0F 1E Sr");
}

TEST(I2cWireDrv, writeSendsRegisterAndDataThenStop)
{
    FakeWire wire;
    I2cWireDrv bus(wire);

    const uint8_t data[2] = {0x1C, 0x20};
    EXPECT_TRUE(bus.write(0x68, 0x0E, 1, data, 2));
    EXPECT_EQ(wire.log.size(), 1u);
    EXPECT_EQ(wire.log[0], "W 68: 0E 1C 20 P");
}

TEST(I2cWireDrv, nackIsNotFatal)
{
    FakeWire wire;
    FakeReporter reporter;
    I2cWireDrv bus(wire);
    bus.setErrorReporter(reporter);
    wire.endResults = {WIRE_NACK_ADDRESS};

    uint8_t data = 0;
    EXPECT_FALSE(bus.read(0x50, 0, 1, &data, 1));
    EXPECT_EQ(bus.lastResult(), I2cResult::NACK_ADDRESS);
    EXPECT_FALSE(bus.hasFatalFailure());
    EXPECT_TRUE(reporter.fatals.empty());
    EXPECT_EQ(wire.log.size(), 1u); // did not try to read after the NACK
}

TEST(I2cWireDrv, timeoutIsFatalAndReportedOnlyOnce)
{
    FakeWire wire;
    FakeReporter reporter;
    I2cWireDrv bus(wire);
    bus.setErrorReporter(reporter);
    wire.endResults = {WIRE_TIMEOUT, WIRE_TIMEOUT};

    uint8_t data = 0;
    EXPECT_FALSE(bus.read(0x68, 0, 1, &data, 1));
    EXPECT_FALSE(bus.read(0x68, 0, 1, &data, 1));

    EXPECT_EQ(bus.lastResult(), I2cResult::TIMEOUT);
    EXPECT_TRUE(bus.hasFatalFailure());
    EXPECT_EQ(reporter.fatals.size(), 1u);
    EXPECT_EQ(reporter.fatals[0].first, Subsystem::I2C_BUS);
}

TEST(I2cWireDrv, shortReadWithTimeoutFlagIsTimeout)
{
    FakeWire wire;
    I2cWireDrv bus(wire);
    wire.requestResults = {0};
    wire.timeoutFlag = true;

    uint8_t data[2];
    EXPECT_FALSE(bus.read(0x68, 0x11, 1, data, 2));
    EXPECT_EQ(bus.lastResult(), I2cResult::TIMEOUT);
}

TEST(I2cWireDrv, failureBeforeReporterIsConnectedIsReplayed)
{
    FakeWire wire;
    FakeReporter reporter;
    I2cWireDrv bus(wire);
    wire.endResults = {WIRE_OTHER_ERROR};

    uint8_t data = 0;
    EXPECT_FALSE(bus.read(0x68, 0, 1, &data, 1)); // nobody listening yet

    bus.setErrorReporter(reporter);
    EXPECT_EQ(reporter.fatals.size(), 1u);
}

TEST(I2cWireDrv, transferLargerThanWireBufferIsRejected)
{
    FakeWire wire;
    I2cWireDrv bus(wire);

    // 2 address bytes + 31 data bytes = 33 > 32
    uint8_t data[31] = {0};
    EXPECT_FALSE(bus.write(0x57, 0, 2, data, sizeof(data)));
    EXPECT_EQ(bus.lastResult(), I2cResult::TOO_LONG);
    EXPECT_TRUE(wire.log.empty()); // nothing went on the wire

    EXPECT_TRUE(bus.write(0x57, 0, 2, data, 30));
}

TEST(I2cWireDrv, programmingErrorsStopTheProgram)
{
    FakeWire wire;
    I2cWireDrv bus(wire);
    uint8_t data = 0;

    EXPECT_ASSERT(bus.read(0xD0, 0, 1, &data, 1));    // 8-bit address of DS3231 by mistake
    EXPECT_ASSERT(bus.read(0x68, 0, 3, &data, 1));    // 3-byte register address
    EXPECT_ASSERT(bus.read(0x68, 0, 1, nullptr, 1));
    EXPECT_ASSERT(bus.read(0x68, 0, 1, &data, 0));
}

TEST(I2cWireDrv, everyValidSevenBitAddressIsAccepted)
{
    // A check like (address & (1 << (sizeof(address) - 1))) looks like
    // "top bit is free", but sizeof counts BYTES: it tests bit 1 and would
    // reject 0x57, 0x6A, ... Test the edge cases, not only the happy path.
    FakeWire wire;
    I2cWireDrv bus(wire);
    uint8_t data = 0;
    for (uint16_t address = 0; address <= 0x7F; ++address)
    {
        wire.rxData = {0};
        EXPECT_TRUE(bus.read(address, 0, 1, &data, 1));
    }
}

TEST(I2cWireDrv, lockIsRecursiveAndBalanced)
{
    FakeWire wire;
    I2cWireDrv bus(wire);
    wire.rxData = {1, 2};

    {
        const LockGuard<I2cDrvIf> outer(bus);
        uint8_t data = 0;
        EXPECT_TRUE(bus.read(0x68, 0, 1, &data, 1)); // locks again inside
        EXPECT_TRUE(bus.read(0x68, 1, 1, &data, 1));
    }
    EXPECT_ASSERT(bus.unlock()); // one unlock too many
}
