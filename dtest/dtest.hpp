#ifndef DTEST_HPP
#define DTEST_HPP

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#define TEST_CASE_START(classname)                                          \
  class classname##_FOR_DTEST_ : public dtest::DTest {                      \
   public:                                                                  \
    classname##_FOR_DTEST_(const std::string& name) : dtest::DTest(name) {} \
    void testCases() override

#define TEST_CASE_END(classname) \
  }                              \
  ;                              \
  dtest::DTest* classname##_(new classname##_FOR_DTEST_(#classname));

#define TEST(testname)                                                     \
  dtest::curr_test_name = #testname;                                       \
  std::cout << dtest::message[dtest::RUN] << dtest::curr_class_name << "." \
            << dtest::curr_test_name << std::endl;                         \
  dtest::start_time = std::chrono::high_resolution_clock::now();

#define EXPECTED_EQ(actual, expected)                            \
  {                                                              \
    dtest::end_time = std::chrono::high_resolution_clock::now(); \
    insert();                                                    \
    dtest::expectedImpl(expected, actual, __FILE__, __LINE__);   \
  }

#define EXPECTED_NE(actual, expected)                                \
  {                                                                  \
    dtest::end_time = std::chrono::high_resolution_clock::now();     \
    insert();                                                        \
    dtest::expectedImpl(expected, actual, __FILE__, __LINE__, true); \
  }

#define EXPECTED_TRUE(actual)                                                 \
  {                                                                           \
    dtest::end_time = std::chrono::high_resolution_clock::now();              \
    insert();                                                                 \
    dtest::expectedImpl(true, static_cast<bool>(actual), __FILE__, __LINE__); \
  }

#define EXPECTED_FALSE(actual)                                                 \
  {                                                                            \
    dtest::end_time = std::chrono::high_resolution_clock::now();               \
    insert();                                                                  \
    dtest::expectedImpl(false, static_cast<bool>(actual), __FILE__, __LINE__); \
  }

namespace dtest {
namespace {
size_t curr_class_pass_count = 0;
long long curr_class_time_consume = 0;
}  // namespace

// (testcase, testname)
using CaseTestPair = std::pair<std::string, std::string>;
using FailList = std::vector<CaseTestPair>;
inline FailList fail_list;

using dtime_t = std::chrono::steady_clock::time_point;

inline std::string curr_class_name("");
inline std::string curr_test_name("");
inline dtime_t start_time(std::chrono::high_resolution_clock::now());
inline dtime_t end_time(std::chrono::high_resolution_clock::now());
inline long long total_consume_time(0);

enum {
  FAILED = 0,
  PASSED,
  RUN,
  OK,
  SPLITER,
  THIN_SPLITER,
  THIN_SPLITER_RED,
  MESSAGE_COUNT
};

inline std::string message[MESSAGE_COUNT] = {
    "\x1B[31m[  FAILED  ]\033[0m ",  // FAILED_CENTER
    "\x1B[32m[  PASSED  ]\033[0m ",  // PASSED_CENTER
    "\x1B[32m[ RUN      ]\033[0m ",  // RUN
    "\x1B[32m[       OK ]\033[0m ",  // OK
    "\x1B[32m[==========]\033[0m ",  // SPLICTER
    "\x1B[32m[----------]\033[0m ",  // THIN_SPLITER
    "\x1B[31m[----------]\033[0m "   // THIN_SPLITER_RED
};

enum class Result { FAILED, PASSED };

inline long long calTime() {
  auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);
  curr_class_time_consume += elapsed_time.count();
  return elapsed_time.count();
}

template <typename T>
void expectedImpl(T expected,
                  T actual,
                  const char* filename,
                  const int line,
                  const bool reverse = false) {
  std::string msg;
  bool result = reverse ? (actual != expected) : (actual == expected);

  if (result) {
    ++curr_class_pass_count;
    msg = message[OK];
  } else {
    std::cout << filename << ":" << line << ": Failure" << std::endl;
    fail_list.push_back(std::make_pair(curr_class_name, curr_test_name));
    msg = message[FAILED];
  }

  std::cout << msg << curr_class_name << "." << curr_test_name << " ("
            << calTime() << " ms)" << std::endl;
}

class DTest {
 public:
  struct DTable {
    std::vector<DTest*> dtests;
  };
  inline static DTable dtable_;

  DTest(const std::string& name) : name_(name) {
    ++total_case_count;
    dtable_.dtests.push_back(this);
  }
  virtual ~DTest() {
    size_t fail_count = count_ - curr_class_pass_count;
    (fail_count > 0) ? std::cout << message[THIN_SPLITER_RED]
                     : std::cout << message[THIN_SPLITER];

    std::cout << count_ << " test(s) from " << name_ << ". "
              << curr_class_pass_count << " passed, "
              << count_ - curr_class_pass_count << " failed"
              << " (" << curr_class_time_consume << " ms total)" << std::endl;

    std::cout << std::endl;

    total_consume_time += curr_class_time_consume;

    curr_class_pass_count = 0;
    curr_class_time_consume = 0;
    total_count_ += count_;
  }

  void test() {
    curr_class_name = name_;

    std::cout << dtest::message[dtest::THIN_SPLITER] << "Running tests from "
              << name_ << std::endl;

    testCases();
  }

  static size_t getTotalCaseCount() { return total_case_count; }
  static size_t getTotalTestCount() { return total_count_; }

 protected:
  virtual void testCases() = 0;

  void insert() { ++count_; }

 private:
  std::string name_;
  size_t count_ = 0;

  static size_t total_case_count;
  static size_t total_count_;
};

}  // namespace dtest

#endif  // DTEST_HPP