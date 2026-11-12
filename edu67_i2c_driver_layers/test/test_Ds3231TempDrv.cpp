#include "TinyTest.h"
#include "Fakes.h"

#include "Ds3231TempDrv.h"

namespace {

/** A bus with a simulated DS3231 on it (registers 0x00..0x12). */
struct Ds3231Fixture
{
    Ds3231Fixture()
    {
        bus.addDevice(DS3231_ADDRESS, 0x13);
        setTemperatureRegisters(0x19, 0x00); // 25.00 C
    }

    void setTemperatureRegisters(uint8_t msbP, uint8_t lsbP)
    {
        registers()[DS3231_REG_TEMP_MSB] = msbP;
        registers()[DS3231_REG_TEMP_LSB] = lsbP;
    }

    std::vector<uint8_t>& registers() { return bus.devices[DS3231_ADDRESS].memory; }

    int32_t readTemperature()
    {
        int32_t value = 0x7FFFFFFF;
        EXPECT_TRUE(sensor.getTemperature(value));
        return value;
    }

    FakeI2cBus bus;
    Ds3231TempDrv sensor{bus};
};

} // anonymous namespace

TEST(Ds3231TempDrv, positiveTemperature)
{
    Ds3231Fixture f;
    f.setTemperatureRegisters(0x19, 0x40); // 25 + 1 quarter
    EXPECT_EQ(f.readTemperature(), 252500);
}

TEST(Ds3231TempDrv, quarterDegreeResolution)
{
    Ds3231Fixture f;
    f.setTemperatureRegisters(0x00, 0xC0); // 3 quarters
    EXPECT_EQ(f.readTemperature(), 7500);
}

TEST(Ds3231TempDrv, negativeTemperatures)
{
    Ds3231Fixture f;
    f.setTemperatureRegisters(0xE7, 0x00); // -25.00 C
    EXPECT_EQ(f.readTemperature(), -250000);

    f.setTemperatureRegisters(0xFF, 0xC0); // -0.25 C: the smallest negative value
    EXPECT_EQ(f.readTemperature(), -2500);

    f.setTemperatureRegisters(0xD8, 0x00); // -40 C: datasheet minimum
    EXPECT_EQ(f.readTemperature(), -400000);
}

TEST(Ds3231TempDrv, maximumTemperatureNeeds32BitArithmetic)
{
    Ds3231Fixture f;
    f.setTemperatureRegisters(0x55, 0x00); // +85 C
    EXPECT_EQ(f.readTemperature(), 850000);

    // What the same multiplication gives in 16-bit "int", like on the Uno:
    const int16_t quarters = 85 * 4;
    const int16_t on16bitCpu = static_cast<int16_t>(quarters * 2500);
    EXPECT_FALSE(on16bitCpu == 850000); // garbage (-2672), not 850000
}

TEST(Ds3231TempDrv, readsBothTemperatureRegistersInOneTransfer)
{
    Ds3231Fixture f;
    f.readTemperature();
    EXPECT_EQ(f.bus.log[0], "R 68 @11 x2");
}

TEST(Ds3231TempDrv, startsNewConversionWhenIdle)
{
    Ds3231Fixture f;
    f.registers()[DS3231_REG_CONTROL] = 0x1C; // power-on default

    f.readTemperature();

    EXPECT_EQ(f.bus.log.size(), 3u);
    EXPECT_EQ(f.bus.log[1], "R 68 @0E x2");     // control + status in one go
    EXPECT_EQ(f.bus.log[2], "W 68 @0E x1");
    EXPECT_EQ(f.registers()[DS3231_REG_CONTROL], 0x3C); // CONV set, other bits kept
}

TEST(Ds3231TempDrv, doesNotStartConversionWhileBusy)
{
    Ds3231Fixture f;
    f.registers()[DS3231_REG_STATUS] = DS3231_STATUS_BSY;
    f.readTemperature();
    EXPECT_EQ(f.bus.log.size(), 2u); // no write

    Ds3231Fixture g;
    g.registers()[DS3231_REG_CONTROL] = DS3231_CONTROL_CONV;
    g.readTemperature();
    EXPECT_EQ(g.bus.log.size(), 2u);
}

TEST(Ds3231TempDrv, holdsTheBusLockForTheWholeSequence)
{
    Ds3231Fixture f;
    f.readTemperature();
    for (int depth : f.bus.lockDepthSeen)
    {
        EXPECT_TRUE(depth >= 1);
    }
    EXPECT_EQ(f.bus.lockDepth, 0); // and released it afterwards
}

TEST(Ds3231TempDrv, initWritesNothing)
{
    Ds3231Fixture f;
    EXPECT_TRUE(f.sensor.init());
    EXPECT_EQ(f.bus.log.size(), 1u);
    EXPECT_EQ(f.bus.log[0], "R 68 @0E x1");
}

TEST(Ds3231TempDrv, missingChipIsFatalAndReportedOnce)
{
    FakeI2cBus bus; // nothing connected
    FakeReporter reporter;
    Ds3231TempDrv sensor(bus);
    sensor.setErrorReporter(reporter);

    int32_t value = 0;
    EXPECT_FALSE(sensor.init());
    EXPECT_FALSE(sensor.getTemperature(value));
    EXPECT_FALSE(sensor.getTemperature(value));

    EXPECT_TRUE(sensor.hasFatalFailure());
    EXPECT_EQ(reporter.fatals.size(), 1u);
    EXPECT_EQ(reporter.fatals[0].second, HwError::DS3231_FAILURE);
    EXPECT_EQ(reporter.logs.size(), 1u);
    EXPECT_EQ(bus.log.size(), 1u); // after the failure it stops using the bus
}

TEST(Ds3231TempDrv, failureInTheMiddleOfTheSequence)
{
    Ds3231Fixture f;
    f.bus.failAfter = 1; // temperature read works, control read fails

    int32_t value = 0;
    EXPECT_FALSE(f.sensor.getTemperature(value));
    EXPECT_TRUE(f.sensor.hasFatalFailure());
    EXPECT_EQ(f.bus.lockDepth, 0); // LockGuard unlocked on the early return
}

TEST(Ds3231TempDrv, selfTest)
{
    Ds3231Fixture f;
    EXPECT_TRUE(f.sensor.selfTest());

    Ds3231Fixture hot;
    hot.setTemperatureRegisters(0x64, 0x00); // 100 C: outside -40..85
    EXPECT_FALSE(hot.sensor.selfTest());
}
