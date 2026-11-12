#include "TinyTest.h"

#include "Platform.h"

int main()
{
    int failedTests = 0;
    for (const TinyTest::TestCase& test : TinyTest::registry())
    {
        const int failuresBefore = TinyTest::failureCount();
        try
        {
            test.function();
        }
        catch (const EduAssertFailure& e)
        {
            TinyTest::reportFailure(std::string("unexpected EDU_ASSERT: ") + e.what(), "?", 0);
        }
        const bool passed = (TinyTest::failureCount() == failuresBefore);
        std::printf("[%s] %s\n", passed ? "  OK  " : " FAIL ", test.name);
        if (!passed)
        {
            ++failedTests;
        }
    }

    std::printf("\n%d tests, %d failed\n", static_cast<int>(TinyTest::registry().size()), failedTests);
    return (failedTests == 0) ? 0 : 1;
}
