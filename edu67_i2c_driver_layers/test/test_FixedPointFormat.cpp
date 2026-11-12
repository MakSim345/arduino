#include "TinyTest.h"

#include "FixedPointFormat.h"

#include <string>

namespace {

std::string format(int32_t valueE4P)
{
    char text[FIXED_E4_TEXT_SIZE];
    formatFixedE4(valueE4P, text, sizeof(text));
    return text;
}

} // anonymous namespace

TEST(FixedPointFormat, positive)
{
    EXPECT_EQ(format(255000), "25.5000");
    EXPECT_EQ(format(250000), "25.0000");
    EXPECT_EQ(format(625), "0.0625");
    EXPECT_EQ(format(0), "0.0000");
}

TEST(FixedPointFormat, negativeBelowOneDegreeKeepsItsSign)
{
    EXPECT_EQ(format(-2500), "-0.2500");
    EXPECT_EQ(format(-250000), "-25.0000");
}

TEST(FixedPointFormat, extremes)
{
    EXPECT_EQ(format(INT32_MAX), "214748.3647");
    EXPECT_EQ(format(INT32_MIN), "-214748.3648");
}

TEST(FixedPointFormat, tooSmallBufferIsAProgrammingError)
{
    char text[8];
    EXPECT_ASSERT(formatFixedE4(0, text, sizeof(text)));
}
