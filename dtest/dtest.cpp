#include "dtest.hpp"

int main() {
  size_t total_test = dtest::DTest::dtable_.dtests.size();
  std::cout << dtest::message[dtest::SPLITER] << "Running " << total_test
            << " test cases." << std::endl;

  for (auto testclass : dtest::DTest::dtable_.dtests) {
    testclass->test();
    delete testclass;
  }
}