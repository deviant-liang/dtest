#include "dtest.hpp"

namespace dtest {
size_t DTest::total_case_count = 0;
size_t DTest::total_count_ = 0;
}  // namespace dtest

int main() {
  std::cout << "Running main() from " << __FILE__ << std::endl;

  std::cout << dtest::message[dtest::SPLITER] << "Running "
            << dtest::DTest::getTotalCaseCount() << " test case(s)"
            << std::endl;

  for (auto testclass : dtest::DTest::dtable_.dtests) {
    testclass->test();
    delete testclass;
  }

  std::cout << dtest::message[dtest::SPLITER]
            << dtest::DTest::getTotalTestCount() << " test(s) ran from "
            << dtest::DTest::getTotalCaseCount() << " test case(s). ("
            << dtest::total_consume_time << " ms total)" << std::endl;

  size_t total_pass_test =
      dtest::DTest::getTotalTestCount() - dtest::fail_list.size();
  std::cout << dtest::message[dtest::PASSED] << total_pass_test << " test(s)."
            << std::endl;

  if (!dtest::fail_list.empty()) {
    std::cout << dtest::message[dtest::FAILED] << dtest::fail_list.size()
              << " test(s), listed below:" << std::endl;

    for (const auto& case_test_pair : dtest::fail_list) {
      std::cout << dtest::message[dtest::FAILED] << case_test_pair.first << "."
                << case_test_pair.second << std::endl;
    }
  }

  std::cout << std::endl;
}