#include "simple_test.h"

int SimpleTest::tests_passed_ = 0;
int SimpleTest::tests_failed_ = 0;

void SimpleTest::AssertTrue(bool condition, const std::string& message) {
    if (condition) {
        std::cout << "PASS: " << message << std::endl;
        tests_passed_++;
    }
    else {
        std::cout << "FAIL: " << message << std::endl;
        tests_failed_++;
    }
}

void SimpleTest::AssertEquals(int expected, int actual, const std::string& message) {
    if (expected == actual) {
        std::cout << "PASS: " << message << " (expected " << expected << ", got " << actual << ")" << std::endl;
        tests_passed_++;
    }
    else {
        std::cout << "FAIL: " << message << " (expected " << expected << ", got " << actual << ")" << std::endl;
        tests_failed_++;
    }
}

void SimpleTest::AssertEquals(double expected, double actual, const std::string& message) {
    if (std::abs(expected - actual) < 1e-9) {
        std::cout << "PASS: " << message << " (expected " << expected << ", got " << actual << ")" << std::endl;
        tests_passed_++;
    }
    else {
        std::cout << "FAIL: " << message << " (expected " << expected << ", got " << actual << ")" << std::endl;
        tests_failed_++;
    }
}

void SimpleTest::AssertArrayEquals(const std::vector<int>& expected,
    const std::vector<int>& actual,
    const std::string& message) {
    if (expected == actual) {
        std::cout << "PASS: " << message << std::endl;
        tests_passed_++;
    }
    else {
        std::cout << "FAIL: " << message << std::endl;
        tests_failed_++;
    }
}

void SimpleTest::PrintResults() {
    std::cout << "\n=== TEST RESULTS ===" << std::endl;
    std::cout << "Passed: " << tests_passed_ << std::endl;
    std::cout << "Failed: " << tests_failed_ << std::endl;
    std::cout << "Total: " << (tests_passed_ + tests_failed_) << std::endl;

    if (tests_failed_ == 0) {
        std::cout << "ALL TESTS PASSED!" << std::endl;
    }
    else {
        std::cout << "SOME TESTS FAILED!" << std::endl;
    }
}