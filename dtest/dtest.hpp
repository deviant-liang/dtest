#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#define TEST_AREA int main()

#define RUN_TEST(classname)           \
  {                                   \
    classname classname##_;           \
    classname##_.setName(#classname); \
    classname##_.testCases();         \
  }

#define TEST_CASES void testCases() override

#define TEST(testname)                                                     \
  dtest::curr_test_name = #testname;                                       \
  std::cout << dtest::message[dtest::RUN] << dtest::curr_class_name << "." \
            << dtest::curr_test_name << std::endl;                         \
  dtest::start_time = std::chrono::high_resolution_clock::now();

#define EXPECTED_EQ(actual, expected)                          \
  dtest::end_time = std::chrono::high_resolution_clock::now(); \
  insert(dtest::curr_test_name, dtest::assertImpl(actual, expected));

namespace dtest {
namespace {
size_t curr_class_pass_count = 0;
long long curr_class_time_consume = 0;
}  // namespace

using dtime_t = std::chrono::steady_clock::time_point;

std::string curr_class_name("");
std::string curr_test_name("");
dtime_t start_time = std::chrono::high_resolution_clock::now();
dtime_t end_time = std::chrono::high_resolution_clock::now();

enum {
  FAILED = 0,
  PASSED,
  FAILED_CENTER,
  PASSED_CENTER,
  RUN,
  SPLICTER,
  THIN_SPLITER,
  MESSAGE_COUNT
};

std::string message[MESSAGE_COUNT] = {
    "\x1B[31m[     FAILED ]\033[0m ",  // FAILED
    "\x1B[32m[     PASSED ]\033[0m ",  // PASSED
    "\x1B[31m[   FAILED   ]\033[0m ",  // FAILED_CENTER
    "\x1B[32m[   PASSED   ]\033[0m ",  // PASSED_CENTER
    "\x1B[32m[ RUN        ]\033[0m ",  // RUN
    "\x1B[32m[============]\033[0m ",  // SPLICTER
    "\x1B[32m[------------]\033[0m "   // THIN_SPLITER
};

enum class Result { FAILED, PASSED };

long long calTime() {
  auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);
  curr_class_time_consume += elapsed_time.count();
  return elapsed_time.count();
}

template <typename T>
Result assertImpl(T actual, T expected) {
  bool result = (actual == expected);
  if (result) {
    std::cout << message[PASSED] << curr_class_name << "." << curr_test_name
              << " (" << calTime() << " ms)" << std::endl;
    curr_class_pass_count++;
  } else {
    std::cout << message[FAILED] << curr_class_name << "." << curr_test_name
              << " (" << calTime() << " ms)" << std::endl;
  }
  return result ? Result::PASSED : Result::FAILED;
}

struct CaseResult {
  std::string testname;
  Result result;
};

class DTest {
 public:
  explicit DTest() = default;
  virtual ~DTest() {
    size_t fail_count = count_ - curr_class_pass_count;
    (fail_count > 0) ? std::cout << message[FAILED_CENTER]
                     : std::cout << message[PASSED_CENTER];

    std::cout << count_ << " test(s) from " << name_ << ", "
              << curr_class_pass_count << " passed, "
              << count_ - curr_class_pass_count << " failed"
              << " (total: " << curr_class_time_consume << " ms)" << std::endl;
    curr_class_pass_count = 0;
    curr_class_time_consume = 0;
  }

  void setName(const std::string& name) {
    std::cout << message[SPLICTER] << "start test " << name << std::endl;
    curr_class_name = name;
    name_ = name;
  }
  const std::string& getName() const { return name_; }

  const std::vector<CaseResult>& getCaseResult() const { return case_results_; }

 protected:
  virtual void testCases() = 0;

  void insert(const std::string& name, Result res) {
    ++count_;
    case_results_.emplace_back(CaseResult{.testname = name, .result = res});
  }

 private:
  std::vector<CaseResult> case_results_;
  std::string name_;
  size_t count_ = 0;
};

}  // namespace dtest