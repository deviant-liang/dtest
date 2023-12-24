# DTest Framework

DTest is a lightweight C++ testing framework for writing and executing unit tests.

## Features

- Simple and easy-to-use syntax for defining test cases.
- Color-coded output for easy identification of test results.
- Detailed test summary, including the number of tests passed, failed, and the total time consumed.

## Usage

1. Copy the `DTest.hpp` and `dtest.cpp` files into your project directory.

2. Include `DTest.hpp` in your test files:

    ```cpp
    #include "DTest.hpp"
    ```

3. Define your test cases using the provided macros:

    ```cpp
    TEST_CASE_START(MyTestClass)
        // Your test cases go here
    TEST_CASE_END(MyTestClass)
    ```

4. Within each test case, use the `TEST` and `EXPECTED_` macros to define your individual tests:

    ```cpp
    TEST(MyTestName)
        // Your test logic goes here
        EXPECTED_EQ(actual, expected);
    ```

5. Compile your test files along with `dtest.cpp` and run the executable.

6. View the color-coded test results and summary in the console output.

## Example

Here's a simple example of a test file using DTest:

```cpp
#include "DTest.hpp"

TEST_CASE_START(MyTestClass)
    TEST(MyTestName)
        int result = 42;
        EXPECTED_EQ(result, 42);
TEST_CASE_END(MyTestClass)

int main() {
    std::cout << "Running main() from " << __FILE__ << std::endl;

    for (auto testclass : dtest::DTest::dtable_.dtests) {
        testclass->test();
        delete testclass;
    }

    // Display test summary
    std::cout << dtest::message[dtest::SPLITER]
              << dtest::DTest::getTotalTestCount() << " test(s) ran from "
              << dtest::DTest::getTotalCaseCount() << " test case(s). ("
              << dtest::total_consume_time << " ms total)" << std::endl;

    size_t total_pass_test =
        dtest::DTest::getTotalTestCount() - dtest::fail_list.size();
    std::cout << dtest::message[dtest::PASSED] << total_pass_test << " test(s)."
              << std::endl;

    // Display failed tests, if any
    if (!dtest::fail_list.empty()) {
        std::cout << dtest::message[dtest::FAILED] << dtest::fail_list.size()
                  << " test(s), listed below:" << std::endl;

        for (const auto& case_test_pair : dtest::fail_list) {
            std::cout << dtest::message[dtest::FAILED] << case_test_pair.first
                      << "." << case_test_pair.second << std::endl;
        }
    }

    std::cout << std::endl;
    return 0;
}
