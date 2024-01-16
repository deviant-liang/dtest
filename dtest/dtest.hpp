#ifndef DTEST_HPP
#define DTEST_HPP

#include <chrono>
#include <string>
#include <vector>

#define TEST_CASE(casename)                      \
  class casename##_DTEST : public dtest::DTest { \
   public:                                       \
    std::string getName() const override {       \
      return #casename;                          \
    }                                            \
                                                 \
   protected:                                    \
    void testCases() override;                   \
  };                                             \
  casename##_DTEST casename##_DTEST_INST;        \
  void casename##_DTEST::testCases()

#define TEST(testname) startTest(#testname);

#define EXPECT_EQ(actual, expected)                            \
  dtest::end_time = std::chrono::high_resolution_clock::now(); \
  dtest::handleResult(dtest::expectImpl(expected, actual), __FILE__, __LINE__);

#define EXPECT_NE(actual, expected)                                        \
  dtest::end_time = std::chrono::high_resolution_clock::now();             \
  dtest::handleResult(dtest::expectImpl(expected, actual, true), __FILE__, \
                      __LINE__);

#define EXPECT_TRUE(actual)                                               \
  dtest::end_time = std::chrono::high_resolution_clock::now();            \
  dtest::handleResult(dtest::expectImpl(true, static_cast<bool>(actual)), \
                      __FILE__, __LINE__);

#define EXPECT_FALSE(actual)                                               \
  dtest::end_time = std::chrono::high_resolution_clock::now();             \
  dtest::handleResult(dtest::expectImpl(false, static_cast<bool>(actual)), \
                      __FILE__, __LINE__);

namespace dtest {
using CaseTestPair =
    std::pair<std::string, std::string>;  // (testcase, testname)
using FailList = std::vector<CaseTestPair>;
inline FailList fail_list;

inline size_t curr_class_pass_count = 0;
inline long long curr_class_time_consume = 0;
inline std::string curr_class_name("");
inline std::string curr_test_name("");

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

inline long long calTime() {
  auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);
  curr_class_time_consume += elapsed_time.count();
  return elapsed_time.count();
}

template <typename T>
bool expectImpl(T expected, T actual, const bool reverse = false) {
  return reverse ? (actual != expected) : (actual == expected);
}

inline void handleResult(bool result, const char* filename, const int line) {
  std::string msg;
  if (result) {
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
    // pointer to global object
    std::vector<DTest*> dtests;
  };
  inline static DTable dtable_;

  DTest() noexcept;

  virtual ~DTest() = default;

  void shutdown();

  void test();

  static size_t getTotalCaseCount() { return total_case_count_; }
  static size_t getTotalTestCount() { return total_count_; }

 protected:
  virtual std::string getName() const = 0;
  virtual void testCases() = 0;

  void startTest(const char* testname);

 private:
  void setup();

  size_t count_ = 0;

  static size_t total_case_count_;
  static size_t total_count_;
};

}  // namespace dtest

#endif  // DTEST_HPP
