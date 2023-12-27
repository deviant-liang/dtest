#ifndef DTEST_HPP
#define DTEST_HPP

#include <chrono>
#include <string>
#include <vector>

#define TEST_CASE(classname)                                         \
  class classname##_FOR_DTEST_ : public dtest::DTest {               \
   public:                                                           \
    std::string getName() const override {                           \
      return #classname;                                             \
    }                                                                \
    void testCases() override;                                       \
  };                                                                 \
  dtest::DTest* classname##_FOR_DTEST__(new classname##_FOR_DTEST_); \
  void classname##_FOR_DTEST_ ::testCases()

#define TEST(testname) dtest::startTest(#testname);

#define EXPECTED_EQ(actual, expected)                           \
  dtest::end_time = std::chrono::high_resolution_clock::now();  \
  insert();                                                     \
  dtest::curr_expected = dtest::expectedImpl(expected, actual); \
  dtest::handleResult(__FILE__, __LINE__);

#define EXPECTED_NE(actual, expected)                                 \
  dtest::end_time = std::chrono::high_resolution_clock::now();        \
  insert();                                                           \
  dtest::curr_expected = dtest::expectedImpl(expected, actual, true); \
  dtest::handleResult(__FILE__, __LINE__);

#define EXPECTED_TRUE(actual)                                                  \
  dtest::end_time = std::chrono::high_resolution_clock::now();                 \
  insert();                                                                    \
  dtest::curr_expected = dtest::expectedImpl(true, static_cast<bool>(actual)); \
  dtest::handleResult(__FILE__, __LINE__);

#define EXPECTED_FALSE(actual)                                 \
  dtest::end_time = std::chrono::high_resolution_clock::now(); \
  insert();                                                    \
  dtest::curr_expected =                                       \
      dtest::expectedImpl(false, static_cast<bool>(actual));   \
  dtest::handleResult(__FILE__, __LINE__);

namespace dtest {
using CaseTestPair =
    std::pair<std::string, std::string>;  // (testcase, testname)
using FailList = std::vector<CaseTestPair>;
inline FailList fail_list;

inline size_t curr_class_pass_count = 0;
inline long long curr_class_time_consume = 0;
inline std::string curr_class_name("");
inline std::string curr_test_name("");

inline bool curr_expected = false;

using dtime_t = std::chrono::steady_clock::time_point;
inline dtime_t start_time;
inline dtime_t end_time;

inline long long total_consume_time = 0;

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

inline void startTest(const char* testname) {
  curr_test_name = testname;
  printf("%s%s.%s\n", message[RUN].c_str(), curr_class_name.c_str(),
         curr_test_name.c_str());
  start_time = std::chrono::high_resolution_clock::now();
}

inline long long calTime() {
  auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);
  curr_class_time_consume += elapsed_time.count();
  return elapsed_time.count();
}

template <typename T>
bool expectedImpl(T expected, T actual, const bool reverse = false) {
  return reverse ? (actual != expected) : (actual == expected);
}

inline void handleResult(const char* filename, const int line) {
  std::string msg;
  if (curr_expected) {
    ++curr_class_pass_count;
    msg = message[OK];
  } else {
    printf("%s:%d: Failure\n", filename, line);
    fail_list.push_back(std::make_pair(curr_class_name, curr_test_name));
    msg = message[FAILED];
  }

  printf("%s%s.%s (%lld ms)\n", msg.c_str(), curr_test_name.c_str(),
                           curr_test_name.c_str(), calTime());
}

class DTest {
 public:
  struct DTable {
    std::vector<DTest*> dtests;
  };
  inline static DTable dtable_;

  DTest();

  virtual ~DTest();

  void test();

  static size_t getTotalCaseCount() { return total_case_count_; }
  static size_t getTotalTestCount() { return total_count_; }

 protected:
  virtual std::string getName() const { return ""; }
  virtual void testCases() = 0;

  void insert() { ++count_; }

 private:
  size_t count_ = 0;

  static size_t total_case_count_;
  static size_t total_count_;
};

}  // namespace dtest

#endif  // DTEST_HPP
