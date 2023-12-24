#include "dtest.hpp"

#include <format>

namespace dtest {
namespace {
class DTestSetup {
 public:
  DTestSetup() {
    whereIsMain();
    runTestCases();

    mainLoop();
  }

  ~DTestSetup() {
    testRan();
    totalPassTests();
    failList();

    end();
  }

 private:
  void mainLoop() const {
    for (auto testclass : DTest::dtable_.dtests) {
      testclass->test();
      delete testclass;
    }
  }

  void whereIsMain() const {
    std::cout << std::format("Running main() from {}\n", __FILE__);
  }

  void runTestCases() const {
    std::cout << std::format("{}Running {} test cases(s).\n", message[SPLITER],
                             DTest::getTotalCaseCount());
  }

  void testRan() const {
    std::cout << std::format(
        "{}{} test(s) ran from {} test case(s). ({} ms total)\n",
        message[SPLITER], DTest::getTotalTestCount(),
        DTest::getTotalCaseCount(), total_consume_time);
  }

  void totalPassTests() const {
    std::cout << std::format("{}{} test(s).\n", message[PASSED],
                             DTest::getTotalTestCount() - fail_list.size());
  }

  void failList() const {
    if (!fail_list.empty()) {
      std::cout << std::format("{}{} test(s), listed below:\n", message[FAILED],
                               fail_list.size());
      for (const auto& case_test_pair : fail_list) {
        std::cout << std::format("{}{}.{}\n", message[FAILED],
                                 case_test_pair.first, case_test_pair.second);
      }
    }
  }

  void end() const { std::cout << std::endl; }
};
}  // namespace

size_t DTest::total_case_count_ = 0;
size_t DTest::total_count_ = 0;

DTest::DTest(const std::string& name) : name_(name) {
  ++total_case_count_;
  dtable_.dtests.push_back(this);
}

DTest::~DTest() {
  size_t fail_count = count_ - curr_class_pass_count;
  const std::string& msg =
      (fail_count > 0) ? message[THIN_SPLITER_RED] : message[THIN_SPLITER];

  std::cout << std::format(
      "{}{} test(s) from {}. {} passed, {} failed ({} ms total)\n\n", msg,
      count_, name_, curr_class_pass_count, fail_count,
      curr_class_time_consume);

  total_consume_time += curr_class_time_consume;

  curr_class_pass_count = 0;
  curr_class_time_consume = 0;
  total_count_ += count_;
}

void DTest::test() {
  curr_class_name = name_;

  std::cout << std::format("{}Running tests from {}\n", message[THIN_SPLITER],
                           name_);

  testCases();
}
}  // namespace dtest

int main() {
  dtest::DTestSetup setup;
}
