#include "simple_test.h"
#include "core.h"
#include "min_max_wrapper.h"
#include "average_wrapper.h"

void RunMinMaxTests() {
    std::cout << "=== MinMaxWrapper Tests ===" << std::endl;

    std::vector<int> test1 = { 3, 1, 4, 1, 5, 9, 2, 6 };
    auto result1 = MinMaxWrapper::Calculate(test1);
    TEST_TRUE(result1.success, "MinMax calculation success");
    TEST_EQUAL(1, result1.min_value, "Min value in array {3,1,4,1,5,9,2,6}");
    TEST_EQUAL(9, result1.max_value, "Max value in array {3,1,4,1,5,9,2,6}");

    std::vector<int> test2 = { -1, -2, -3, -4, -5 };
    auto result2 = MinMaxWrapper::Calculate(test2);
    TEST_EQUAL(-5, result2.min_value, "Min value in negative array");
    TEST_EQUAL(-1, result2.max_value, "Max value in negative array");

    std::vector<int> test3 = { 42 };
    auto result3 = MinMaxWrapper::Calculate(test3);
    TEST_EQUAL(42, result3.min_value, "Min value in single element array");
    TEST_EQUAL(42, result3.max_value, "Max value in single element array");

    std::vector<int> test4 = { 5, 5, 5, 5 };
    auto result4 = MinMaxWrapper::Calculate(test4);
    TEST_EQUAL(5, result4.min_value, "Min value in uniform array");
    TEST_EQUAL(5, result4.max_value, "Max value in uniform array");
}

void RunAverageTests() {
    std::cout << "\n=== AverageWrapper Tests ===" << std::endl;

    std::vector<int> test1 = { 1, 2, 3, 4, 5 };
    double avg1 = AverageWrapper::Calculate(test1).value;
    TEST_EQUAL(3.0, avg1, "Average of {1,2,3,4,5}");

    std::vector<int> test2 = { 10, 20, 30 };
    double avg2 = AverageWrapper::Calculate(test2).value;
    TEST_EQUAL(20.0, avg2, "Average of {10,20,30}");

    std::vector<int> test3 = { 7 };
    double avg3 = AverageWrapper::Calculate(test3).value;
    TEST_EQUAL(7.0, avg3, "Average of single element");

    std::vector<int> test4 = { -2, -1, 0, 1, 2 };
    double avg4 = AverageWrapper::Calculate(test4).value;
    TEST_EQUAL(0.0, avg4, "Average of {-2,-1,0,1,2}");
}

void RunArrayProcessorTests() {
    std::cout << "\n=== ArrayProcessor Tests ===" << std::endl;

    std::vector<int> test1 = { 1, 5, 3, 1, 5 };
    ArrayProcessor::ReplaceMinMaxWithAverage(test1, 1, 5, 3.0);
    std::vector<int> expected1 = { 3, 3, 3, 3, 3 };
    TEST_ARRAY_EQUAL(expected1, test1, "Replace min/max with average");

    std::vector<int> test2 = { 10, 20, 30 };
    ArrayProcessor::ReplaceMinMaxWithAverage(test2, 10, 30, 20.0);
    std::vector<int> expected2 = { 20, 20, 20 };
    TEST_ARRAY_EQUAL(expected2, test2, "Replace min/max in three elements");

    std::vector<int> test3 = { 8, 8, 8 };
    ArrayProcessor::ReplaceMinMaxWithAverage(test3, 8, 8, 8.0);
    std::vector<int> expected3 = { 8, 8, 8 };
    TEST_ARRAY_EQUAL(expected3, test3, "No change when min=max=average");
}

void RunEmptyArrayTests() {
    std::cout << "\n=== Empty Array Tests ===" << std::endl;

    std::vector<int> empty;
    auto min_max_result = MinMaxWrapper::Calculate(empty);
    double average = AverageWrapper::Calculate(empty).value;

    TEST_TRUE(!min_max_result.success, "MinMax should fail on empty array");
    TEST_TRUE(average == 0.0, "Average of empty array is 0.0");
}

void RunDataDrivenTests() {
    std::cout << "\n=== Data Driven Tests ===" << std::endl;

    struct TestCase {
        std::vector<int> input;
        int expected_min;
        int expected_max;
        double expected_avg;
        std::string description;
    };

    std::vector<TestCase> test_cases = {
        {{1, 2, 3}, 1, 3, 2.0, "Simple ascending sequence"},
        {{3, 2, 1}, 1, 3, 2.0, "Simple descending sequence"},
        {{2, 2, 2}, 2, 2, 2.0, "All elements equal"},
        {{0, 0, 0}, 0, 0, 0.0, "All zeros"},
        {{-5, 0, 5}, -5, 5, 0.0, "Negative and positive numbers"}
    };

    for (const auto& test_case : test_cases) {
        auto min_max_result = MinMaxWrapper::Calculate(test_case.input);
        double average = AverageWrapper::Calculate(test_case.input).value;

        TEST_EQUAL(test_case.expected_min, min_max_result.min_value,
            test_case.description + " - min");
        TEST_EQUAL(test_case.expected_max, min_max_result.max_value,
            test_case.description + " - max");
        TEST_EQUAL(test_case.expected_avg, average,
            test_case.description + " - average");
    }
}

int main() {
    std::cout << "Running ThreadLab2 Tests..." << std::endl;

    RunMinMaxTests();
    RunAverageTests();
    RunArrayProcessorTests();
    RunEmptyArrayTests();
    RunDataDrivenTests();

    SimpleTest::PrintResults();

    return SimpleTest::GetTestsFailed() > 0 ? 1 : 0;
}