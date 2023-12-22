#include "dtest.hpp"

#include <chrono>
#include <thread>

TEST_CASE_START(ThisIsATestClass) {
  TEST(METHOD_1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::string actual = "actual";
    std::string expected = "expected";
    EXPECTED_EQ(actual, expected);
  }

  TEST(METHOD_2) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::string actual = "actual";
    std::string expected = "actual";
    EXPECTED_EQ(actual, expected);
  }
}
TEST_CASE_END(ThisIsATestClass)

TEST_CASE_START(ThisIsBTestClass) {
  TEST(METHOD_1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::string actual = "actual";
    std::string expected = "actual";
    EXPECTED_EQ(actual, expected);
  }

  TEST(METHOD_2) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::string actual = "actual";
    std::string expected = "actual";
    EXPECTED_EQ(actual, expected);
  }
}
TEST_CASE_END(ThisIsBTestClass)