#ifndef MINITEST_MINITEST_H_
#define MINITEST_MINITEST_H_

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace minitest {

using TestFunction = std::function<void()>;

struct TestInfo {
    std::string case_name;
    std::string test_name;
    TestFunction func;
};

inline std::vector<TestInfo>& GetTests() {
    static std::vector<TestInfo> tests;
    return tests;
}

inline int& FailureCount() {
    static int failures = 0;
    return failures;
}

inline int RunAllTests() {
    for (const auto& t : GetTests()) {
        try {
            t.func();
        } catch (...) {
            ++FailureCount();
            std::cerr << t.case_name << "." << t.test_name << " threw an exception\n";
        }
    }
    if (FailureCount()) {
        std::cerr << FailureCount() << " tests failed\n";
    }
    return FailureCount();
}

struct TestRegistrar {
    TestRegistrar(const char* case_name, const char* test_name, TestFunction func) {
        GetTests().push_back({case_name, test_name, std::move(func)});
    }
};

} // namespace minitest

#define TEST(test_case, test_name)                                                  \
    static void test_case##_##test_name();                                          \
    static ::minitest::TestRegistrar test_case##_##test_name##_registrar(           \
        #test_case, #test_name, &test_case##_##test_name);                          \
    static void test_case##_##test_name()

#define EXPECT_TRUE(cond)                                                           \
    do {                                                                            \
        if (!(cond)) {                                                              \
            ::minitest::FailureCount()++;                                           \
            std::cerr << __FILE__ << ":" << __LINE__                                \
                      << ": Failure EXPECT_TRUE(" #cond ")\n";              \
        }                                                                           \
    } while (0)

#define EXPECT_FALSE(cond) EXPECT_TRUE(!(cond))

#define EXPECT_EQ(val1, val2) EXPECT_TRUE((val1) == (val2))

#define RUN_ALL_TESTS() ::minitest::RunAllTests()

#endif // MINITEST_MINITEST_H_
