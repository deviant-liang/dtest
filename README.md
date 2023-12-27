# DTest Framework

DTest is a lightweight C++ testing framework for writing and executing unit tests.

## Features

- Simple and easy-to-use syntax for defining test cases.
- Color-coded output for easy identification of test results.
- Detailed test summary, including the number of tests passed, failed, and the total time consumed.

## Usage

1. Copy the `dtest.hpp` and `dtest.cpp` files into your project directory.

2. Include `dtest.hpp` in your test files:

    ```cpp
    #include "dtest.hpp"
    ```

3. Define your test cases using the provided macros:

    ```cpp
    TEST_CASE(MyTestClass) {
        // Your test cases go here
    }
    ```

4. Within each test case, use the `TEST` and `EXPECTED_` macros to define your individual tests:

    ```cpp
    TEST(MyTestName) {
        // Your test logic goes here
        EXPECTED_EQ(actual, expected);
    }
    ```

5. Compile your test files along with `dtest.cpp` and run the executable.

6. View the color-coded test results and summary in the console output.

## Example

Here's a simple example of a test file using DTest:

```cpp
// example_unittest.cpp
#include "DTest.hpp"

TEST_CASE(MyTestClass) {
  TEST(MyTestName) {
    int result = 42;
    EXPECTED_EQ(result, 42);
  }
}
```