#include "TinyTest.h"
#include "Fakes.h"

#include "I2cItDrv.h"

TEST(I2cItDrv, registersAndRemovesItsCallbacks)
{
    FakeTwiHal hal;
    FakeClock clock;
    {
        I2cItDrv bus(hal, clock);
        EXPECT_TRUE(hal.pCallbacks != nullptr);
    }
    EXPECT_TRUE(hal.pCallbacks == nullptr);
}

TEST(I2cItDrv, readSucceedsWhenInterruptReportsRxComplete)
{
    FakeTwiHal hal;
    FakeClock clock;
    I2cItDrv bus(hal, clock);
    hal.rxData = {0x19, 0x40};

    uint8_t data[2] = {0, 0};
    EXPECT_TRUE(bus.read(0x68, 0x11, 1, data, 2));

    // The 7-bit address is shifted exactly once, on the way to the HAL.
    EXPECT_EQ(hal.lastAddress8bit, 0xD0);
    EXPECT_EQ(hal.lastRegister, 0x11);
    EXPECT_EQ(hal.lastRegisterSize, 1);
    EXPECT_EQ(hal.lastSize, 2);
    EXPECT_EQ(data[0], 0x19);
    EXPECT_EQ(data[1], 0x40);
}

TEST(I2cItDrv, writeSucceedsWhenInterruptReportsTxComplete)
{
    FakeTwiHal hal;
    FakeClock clock;
    I2cItDrv bus(hal, clock);

    const uint8_t data[2] = {0xCA, 0xFE};
    EXPECT_TRUE(bus.write(0x57, 0x0100, 2, data, 2));
    EXPECT_EQ(hal.lastAddress8bit, 0xAE);
    EXPECT_EQ(hexBytes(hal.written), "CA FE");
}

TEST(I2cItDrv, errorFromInterruptIsReturned)
{
    FakeTwiHal hal;
    FakeClock clock;
    I2cItDrv bus(hal, clock);
    hal.reaction = FakeTwiHal::Reaction::ERROR;
    hal.errorToReport = I2cResult::NACK_ADDRESS;

    uint8_t data = 0;
    EXPECT_FALSE(bus.read(0x50, 0, 1, &data, 1));
    EXPECT_EQ(bus.lastResult(), I2cResult::NACK_ADDRESS);
    EXPECT_FALSE(bus.hasFatalFailure());
}

TEST(I2cItDrv, busErrorFromInterruptIsFatal)
{
    FakeTwiHal hal;
    FakeClock clock;
    FakeReporter reporter;
    I2cItDrv bus(hal, clock);
    bus.setErrorReporter(reporter);
    hal.reaction = FakeTwiHal::Reaction::ERROR;
    hal.errorToReport = I2cResult::BUS_ERROR;

    uint8_t data = 0;
    EXPECT_FALSE(bus.read(0x68, 0, 1, &data, 1));
    EXPECT_TRUE(bus.hasFatalFailure());
    EXPECT_EQ(reporter.fatals.size(), 1u);
}

TEST(I2cItDrv, failureToStartIsFatal)
{
    FakeTwiHal hal;
    FakeClock clock;
    FakeReporter reporter;
    I2cItDrv bus(hal, clock);
    bus.setErrorReporter(reporter);
    hal.startSucceeds = false;

    uint8_t data = 0;
    EXPECT_FALSE(bus.read(0x68, 0, 1, &data, 1));
    EXPECT_EQ(bus.lastResult(), I2cResult::BUSY);
    EXPECT_EQ(reporter.fatals.size(), 1u);
}

TEST(I2cItDrv, noInterruptMeansTimeoutAndAbort)
{
    FakeTwiHal hal;
    FakeClock clock;
    I2cItDrv bus(hal, clock, 50);
    hal.reaction = FakeTwiHal::Reaction::NEVER;

    uint8_t data = 0;
    EXPECT_FALSE(bus.read(0x68, 0, 1, &data, 1));
    EXPECT_EQ(bus.lastResult(), I2cResult::TIMEOUT);
    EXPECT_EQ(hal.aborts, 1);
    EXPECT_TRUE(clock.nowMsM >= 50u); // it really waited
}

TEST(I2cItDrv, lateEventAfterTimeoutDoesNotCompleteTheNextTransfer)
{
    // The trap: transfer 1 times out, then its interrupt arrives late and
    // leaves RX_COMPLETE in the slot. Transfer 2 must not take that
    // stale event as its own - its data was never received!
    FakeTwiHal hal;
    FakeClock clock;
    I2cItDrv bus(hal, clock, 10);
    hal.reaction = FakeTwiHal::Reaction::NEVER;

    uint8_t data = 0;
    EXPECT_FALSE(bus.read(0x68, 0, 1, &data, 1));
    hal.fireRxComplete(); // too late

    EXPECT_FALSE(bus.read(0x68, 0, 1, &data, 1));
    EXPECT_EQ(bus.lastResult(), I2cResult::TIMEOUT);
}

TEST(I2cItDrv, wrongEventIsAnError)
{
    // A confused HAL reports TX_COMPLETE while we wait for a read.
    FakeTwiHal hal;
    FakeClock clock;
    FakeReporter reporter;
    I2cItDrv bus(hal, clock);
    bus.setErrorReporter(reporter);
    hal.reaction = FakeTwiHal::Reaction::WRONG_EVENT;

    uint8_t data = 0;
    EXPECT_FALSE(bus.read(0x68, 0, 1, &data, 1));
    EXPECT_EQ(bus.lastResult(), I2cResult::BUS_ERROR);
    EXPECT_TRUE(!reporter.logs.empty() && reporter.logs[0].find("unexpected event") != std::string::npos);
}

TEST(I2cItDrv, writeOfZeroBytesOnlySendsTheRegisterAddress)
{
    FakeTwiHal hal;
    FakeClock clock;
    I2cItDrv bus(hal, clock);

    const uint8_t unused = 0;
    EXPECT_TRUE(bus.write(0x57, 0x0010, 2, &unused, 0));
    EXPECT_EQ(hal.lastSize, 0);
}
