#include "dtest.hpp"

#include <chrono>
#include <thread>

bool isPrime(int number) {
  if (number <= 1) {
    return false;
  }

  for (int i = 2; i * i <= number; ++i) {
    if (number % i == 0) {
      return false;
    }
  }

  return true;
}

TEST_CASE(IsPrime) {
  TEST(IsPrime3) {
    EXPECTED_TRUE(isPrime(3));
  }

  TEST(IsPrime4) {
    EXPECTED_FALSE(isPrime(4));
  }
}

TEST_CASE(Dummy) {
  TEST(StringEqual1) {
    std::string expected = "hello";
    std::string actual = "hi";

    EXPECTED_EQ(expected, actual);
  }

  TEST(StringEqual2) {
    std::string expected = "hello";
    std::string actual = "hi";

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    EXPECTED_NE(expected, actual);
  }
}