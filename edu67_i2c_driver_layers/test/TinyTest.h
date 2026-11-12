#ifndef EDU67_TINYTEST_H
#define EDU67_TINYTEST_H

/*
 * A 100-line test framework, so the tests need nothing but g++.
 * Same spirit (and macro names) as GoogleTest.
 */

#pragma GCC diagnostic ignored "-Wsign-compare"

#include <cstdio>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace TinyTest {

struct TestCase
{
    const char* name;
    void (*function)();
};

inline std::vector<TestCase>& registry()
{
    static std::vector<TestCase> tests;
    return tests;
}

inline int& failureCount()
{
    static int count = 0;
    return count;
}

struct Registrar
{
    Registrar(const char* nameP, void (*functionP)()) { registry().push_back(TestCase{nameP, functionP}); }
};

inline void reportFailure(const std::string& messageP, const char* fileP, int lineP)
{
    std::printf("    FAILED %s:%d: %s\n", fileP, lineP, messageP.c_str());
    ++failureCount();
}

// Integers and enums print as numbers (uint8_t would print as a character otherwise).
template <typename T>
typename std::enable_if<std::is_integral<T>::value || std::is_enum<T>::value, long long>::type
printable(const T& valueP) { return static_cast<long long>(valueP); }

inline std::string printable(const std::string& valueP) { return "\"" + valueP + "\""; }
inline std::string printable(const char* valueP) { return printable(std::string(valueP)); }

template <typename A, typename B>
void checkEqual(const A& aP, const B& bP, const char* textP, const char* fileP, int lineP)
{
    if (!(aP == bP))
    {
        std::ostringstream message;
        message << textP << "  (" << printable(aP) << " vs " << printable(bP) << ")";
        reportFailure(message.str(), fileP, lineP);
    }
}

} // namespace TinyTest

#define TEST(suite, name) \
    static void suite##_##name(); \
    static TinyTest::Registrar registrar_##suite##_##name(#suite "." #name, &suite##_##name); \
    static void suite##_##name()

#define EXPECT_TRUE(cond) \
    do { if (!(cond)) { TinyTest::reportFailure("expected true: " #cond, __FILE__, __LINE__); } } while (0)

#define EXPECT_FALSE(cond) \
    do { if (cond) { TinyTest::reportFailure("expected false: " #cond, __FILE__, __LINE__); } } while (0)

#define EXPECT_EQ(a, b) TinyTest::checkEqual((a), (b), #a " == " #b, __FILE__, __LINE__)

/** Expects that the statement hits an EDU_ASSERT. */
#define EXPECT_ASSERT(statement) \
    do { \
        bool asserted = false; \
        try { statement; } catch (const EduAssertFailure&) { asserted = true; } \
        if (!asserted) { TinyTest::reportFailure("expected EDU_ASSERT: " #statement, __FILE__, __LINE__); } \
    } while (0)

#endif // EDU67_TINYTEST_H
