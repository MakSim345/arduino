#include "TinyTest.h"
#include "Fakes.h"

#include "At24c32Drv.h"

namespace {

struct EepromFixture
{
    explicit EepromFixture(int busyTransfersP = 0)
    {
        bus.addDevice(AT24C32_ADDRESS, AT24C32_SIZE, AT24C32_PAGE_SIZE, busyTransfersP);
    }

    std::vector<uint8_t>& memory() { return bus.devices[AT24C32_ADDRESS].memory; }

    FakeI2cBus bus;
    FakeClock clock;
    At24c32Drv eeprom{bus, clock};
};

std::vector<uint8_t> sequence(size_t sizeP)
{
    std::vector<uint8_t> bytes(sizeP);
    for (size_t i = 0; i < sizeP; ++i)
    {
        bytes[i] = static_cast<uint8_t>(i + 1);
    }
    return bytes;
}

} // anonymous namespace

TEST(At24c32Drv, usesTwoByteMemoryAddress)
{
    EepromFixture f;
    uint8_t data = 0;
    EXPECT_TRUE(f.eeprom.read(0x0F1E, &data, 1));
    EXPECT_EQ(f.bus.log[0], "R 57 @0F1E x1");
}

TEST(At24c32Drv, writeIsSplitAtPageBoundary)
{
    EepromFixture f;
    const std::vector<uint8_t> data = sequence(4);

    // 0x0F1E..0x0F21 crosses the page end at 0x0F20.
    EXPECT_TRUE(f.eeprom.write(0x0F1E, data.data(), data.size()));

    EXPECT_EQ(f.bus.log[0], "W 57 @0F1E x2");
    EXPECT_EQ(f.bus.log[2], "W 57 @0F20 x2");
    EXPECT_EQ(f.memory()[0x0F1E], 1);
    EXPECT_EQ(f.memory()[0x0F1F], 2);
    EXPECT_EQ(f.memory()[0x0F20], 3);
    EXPECT_EQ(f.memory()[0x0F21], 4);
    EXPECT_EQ(f.memory()[0x0F00], 0); // no wrap-around damage at the page start
}

TEST(At24c32Drv, longWriteIsSplitIntoChunksThatFitTheBusBuffer)
{
    EepromFixture f;
    const std::vector<uint8_t> data = sequence(64);
    EXPECT_TRUE(f.eeprom.write(0x0000, data.data(), data.size()));

    size_t writes = 0;
    for (const std::string& entry : f.bus.log)
    {
        if (entry[0] == 'W')
        {
            ++writes;
            EXPECT_TRUE(entry.find("x16") != std::string::npos);
        }
    }
    EXPECT_EQ(writes, 4u);
    EXPECT_TRUE(std::equal(data.begin(), data.end(), f.memory().begin()));
}

TEST(At24c32Drv, readReturnsWhatWasWritten)
{
    EepromFixture f;
    const std::vector<uint8_t> data = sequence(40);
    EXPECT_TRUE(f.eeprom.write(0x0100 + 5, data.data(), data.size()));

    std::vector<uint8_t> back(40, 0);
    EXPECT_TRUE(f.eeprom.read(0x0100 + 5, back.data(), back.size()));
    EXPECT_TRUE(back == data);
}

TEST(At24c32Drv, waitsForTheWriteCycleWithAckPolling)
{
    EepromFixture f(3); // after each write the chip NACKs 3 times
    const uint8_t data = 0x42;
    EXPECT_TRUE(f.eeprom.write(0x0010, &data, 1));

    // write, 3 NACKed polls, 1 ACKed poll
    EXPECT_EQ(f.bus.log.size(), 5u);
    EXPECT_FALSE(f.eeprom.hasFatalFailure());
}

TEST(At24c32Drv, chipThatStaysBusyIsFatal)
{
    EepromFixture f(1000);
    FakeReporter reporter;
    f.eeprom.setErrorReporter(reporter);

    const uint8_t data = 0x42;
    EXPECT_FALSE(f.eeprom.write(0x0010, &data, 1));
    EXPECT_TRUE(f.eeprom.hasFatalFailure());
    EXPECT_EQ(reporter.fatals.size(), 1u);
    EXPECT_TRUE(reporter.logs[0].find("write cycle timeout") != std::string::npos);
}

TEST(At24c32Drv, holdsTheBusLockDuringTheWholeWrite)
{
    EepromFixture f(2);
    const std::vector<uint8_t> data = sequence(20);
    EXPECT_TRUE(f.eeprom.write(0x001C, data.data(), data.size()));
    for (int depth : f.bus.lockDepthSeen)
    {
        EXPECT_TRUE(depth >= 1);
    }
    EXPECT_EQ(f.bus.lockDepth, 0);
}

TEST(At24c32Drv, outOfRangeIsAProgrammingError)
{
    EepromFixture f;
    uint8_t data[2] = {0, 0};
    EXPECT_ASSERT(f.eeprom.read(AT24C32_SIZE - 1, data, 2));
    EXPECT_ASSERT(f.eeprom.write(AT24C32_SIZE, data, 1));
}
