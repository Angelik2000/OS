#ifndef SIMPLE_TEST_H
#define SIMPLE_TEST_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

class SimpleTest {
private:
    static int tests_passed_;
    static int tests_failed_;

public:
    static void AssertTrue(bool condition, const std::string& message);
    static void AssertEquals(int expected, int actual, const std::string& message);
    static void AssertEquals(double expected, double actual, const std::string& message);
    static void AssertArrayEquals(const std::vector<int>& expected,
        const std::vector<int>& actual,
        const std::string& message);

    static void PrintResults();
    static int GetTestsPassed() { return tests_passed_; }
    static int GetTestsFailed() { return tests_failed_; }

    SimpleTest() = delete;
    ~SimpleTest() = delete;

    SimpleTest(const SimpleTest&) = delete;
    SimpleTest& operator=(const SimpleTest&) = delete;
};

#define TEST_TRUE(condition, message) SimpleTest::AssertTrue(condition, message)
#define TEST_EQUAL(expected, actual, message) SimpleTest::AssertEquals(expected, actual, message)
#define TEST_ARRAY_EQUAL(expected, actual, message) SimpleTest::AssertArrayEquals(expected, actual, message)

#endif