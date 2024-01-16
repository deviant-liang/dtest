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
    for (auto testcase : DTest::dtable_.dtests) {
      testcase->test();
      testcase->shutdown();
    }
  }

  void whereIsMain() const { printf("Running main() from %s\n", __FILE__); }

  void runTestCases() const {
    printf("%sRunning %zu test case(s).\n", message[SPLITER].c_str(),
           DTest::getTotalCaseCount());
  }

  void testRan() const {
    printf("%s%zu test(s) ran from %zu test case(s). (%lld ms total)\n",
           message[SPLITER].c_str(), DTest::getTotalTestCount(),
           DTest::getTotalCaseCount(), total_consume_time);
  }

  void totalPassTests() const {
    printf("%s%zu test(s).\n", message[PASSED].c_str(),
           DTest::getTotalTestCount() - fail_list.size());
  }

  void failList() const {
    if (!fail_list.empty()) {
      printf("%s%zu test(s), listed below:\n", message[FAILED].c_str(),
             fail_list.size());
      for (const auto& case_test_pair : fail_list) {
        printf("%s%s.%s\n", message[FAILED].c_str(),
               case_test_pair.first.c_str(), case_test_pair.second.c_str());
      }
    }
  }

  void end() const { printf("\n"); }
};
}  // namespace

size_t DTest::total_case_count_ = 0;
size_t DTest::total_count_ = 0;

DTest::DTest() noexcept {
  setup();
}

void DTest::shutdown() {
  size_t fail_count = count_ - curr_class_pass_count;
  const std::string& msg =
      (fail_count > 0) ? message[THIN_SPLITER_RED] : message[THIN_SPLITER];

  printf("%s%zu test(s) from %s. %zu passed, %zu failed (%lld ms total)\n\n",
         msg.c_str(), count_, getName().c_str(), curr_class_pass_count,
         fail_count, curr_class_time_consume);

  total_consume_time += curr_class_time_consume;

  curr_class_pass_count = 0;
  curr_class_time_consume = 0;
  total_count_ += count_;
}

void DTest::setup() {
  ++total_case_count_;
  dtable_.dtests.push_back(this);
}

void DTest::test() {
  curr_class_name = getName();

  printf("%sRunning tests from %s\n", message[THIN_SPLITER].c_str(),
         getName().c_str());

  testCases();
}

void DTest::startTest(const char* testname) {
  curr_test_name = testname;
  printf("%s%s.%s\n", message[RUN].c_str(), curr_class_name.c_str(),
         curr_test_name.c_str());

  ++count_;
  start_time = std::chrono::high_resolution_clock::now();
}
}  // namespace dtest

int main() {
  dtest::DTestSetup setup;
}
