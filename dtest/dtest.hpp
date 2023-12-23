#ifndef DTEST_HPP
#define DTEST_HPP

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#define TEST_CASE_START(classname)                             \
  class classname : public dtest::DTest {                      \
   public:                                                     \
    classname(const std::string& name) : dtest::DTest(name) {} \
    void testCases() override

#define TEST_CASE_END(classname) \
  }                              \
  ;                              \
  dtest::DTest* classname##_(new classname(#classname));

#define TEST(testname)                                                     \
  dtest::curr_test_name = #testname;                                       \
  std::cout << dtest::message[dtest::RUN] << dtest::curr_class_name << "." \
            << dtest::curr_test_name << std::endl;                         \
  dtest::start_time = std::chrono::high_resolution_clock::now();

#define EXPECTED_EQ(actual, expected)                          \
  dtest::end_time = std::chrono::high_resolution_clock::now(); \
  dtest::assertImpl(actual, expected);                         \
  insert();

namespace dtest {
namespace {
size_t curr_class_pass_count = 0;
long long curr_class_time_consume = 0;
}  // namespace

using dtime_t = std::chrono::steady_clock::time_point;

inline std::string curr_class_name("");
inline std::string curr_test_name("");
inline dtime_t start_time = std::chrono::high_resolution_clock::now();
inline dtime_t end_time = std::chrono::high_resolution_clock::now();

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
void assertImpl(T actual, T expected) {
  bool result = (actual == expected);

  const std::string& msg = result ? message[OK] : message[FAILED];

  std::cout << msg << curr_class_name << "." << curr_test_name << " ("
            << calTime() << " ms)" << std::endl;

  if (result)
    ++curr_class_pass_count;
}

class DTest {
 public:
  struct DTable {
    std::vector<DTest*> dtests;
  };
  inline static DTable dtable_;

  DTest(const std::string& name) : name_(name) {
    curr_class_name = name;
    dtable_.dtests.push_back(this);
  }
  virtual ~DTest() {
    size_t fail_count = count_ - curr_class_pass_count;
    (fail_count > 0) ? std::cout << message[THIN_SPLITER_RED]
                     : std::cout << message[THIN_SPLITER];

    std::cout << count_ << " test(s) from " << name_ << ". "
              << curr_class_pass_count << " passed, "
              << count_ - curr_class_pass_count << " failed"
              << " (total: " << curr_class_time_consume << " ms)" << std::endl;

    std::cout << std::endl;

    curr_class_pass_count = 0;
    curr_class_time_consume = 0;
    total_count_ += count_;
  }

  void test() {
    std::cout << message[THIN_SPLITER] << "Start to test " << name_
              << std::endl;
    testCases();
  }

  static void init() { total_count_ = 0; }
  static size_t getTotalTestCount() { return total_count_; }

 protected:
  virtual void testCases() = 0;

  void insert() { ++count_; }

 private:
  std::string name_;
  size_t count_ = 0;

  static size_t total_count_;
};

}  // namespace dtest

#endif  // DTEST_HPP