#include "dtest.hpp"

namespace dtest {
size_t DTest::total_count_ = 0;
}

int main() {
  std::cout << "Running main() from " << __FILE__ << std::endl;

  dtest::DTest::init();

  size_t total_test = dtest::DTest::dtable_.dtests.size();
  std::cout << dtest::message[dtest::SPLITER] << "Running " << total_test
            << " test cases." << std::endl;

  for (auto testclass : dtest::DTest::dtable_.dtests) {
    testclass->test();
    delete testclass;
  }

  std::cout << dtest::message[dtest::SPLITER]
            << dtest::DTest::getTotalTestCount() << " test(s) ran."
            << std::endl;
}