#ifndef CORE_H
#define CORE_H

#include <vector>
#include <string>

class ArrayProcessor {
public:
    static void InitializeArray(std::vector<int>& array);
    static void PrintArray(const std::vector<int>& array);
    static void ReplaceMinMaxWithAverage(std::vector<int>& array,
        int min_value,
        int max_value,
        double average);

private:
    ArrayProcessor() = delete;
    ~ArrayProcessor() = delete;

    ArrayProcessor(const ArrayProcessor&) = delete;
    ArrayProcessor& operator=(const ArrayProcessor&) = delete;
};

class InputValidator {
public:
    static bool IsValidSize(int size);
    static bool IsValidArrayElement(int value);

    static constexpr int kMaxArraySize = 1000;
    static constexpr int kMinArraySize = 1;

private:
    InputValidator() = delete;
    ~InputValidator() = delete;

    InputValidator(const InputValidator&) = delete;
    InputValidator& operator=(const InputValidator&) = delete;
};

#endif