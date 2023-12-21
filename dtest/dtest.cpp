#include "dtest.hpp"

#include <chrono>
#include <thread>

class ThisIsATestClass : public dtest::DTest {
 public:
  TEST_CASES {
    TEST(METHOD_1) {
      std::this_thread::sleep_for(std::chrono::milliseconds(300));
      std::string actual = "actual";
      std::string expected = "actual";
      EXPECTED_EQ(actual, expected);
    }

    TEST(METHOD_2) {
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      std::string actual = "actual";
      std::string expected = "expected";
      EXPECTED_EQ(actual, expected);
    }
  }
};

class ThisIsBTestClass : public dtest::DTest {
 public:
  TEST_CASES {
    TEST(METHOD_1) {
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      std::string actual = "actual";
      std::string expected = "actual";
      EXPECTED_EQ(actual, expected);
    }

    TEST(METHOD_2) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      std::string actual = "actual";
      std::string expected = "expected";
      EXPECTED_EQ(actual, expected);
    }
  }
};

TEST_AREA {
  RUN_TEST(ThisIsATestClass);
  RUN_TEST(ThisIsBTestClass);
}