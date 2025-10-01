#include "core.h"
#include <iostream>
#include <limits>

void ArrayProcessor::InitializeArray(std::vector<int>& array) {
    std::cout << "Enter " << array.size() << " array elements:" << std::endl;

    for (size_t i = 0; i < array.size(); ++i) {
        while (true) {
            std::cout << "Element [" << i << "]: ";
            if (std::cin >> array[i]) {
                break;
            }
            else {
                std::cout << "Invalid input. Please enter an integer." << std::endl;
                std::cin.clear();
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            }
        }
    }
}

void ArrayProcessor::PrintArray(const std::vector<int>& array) {
    std::cout << "Array: ";
    for (const auto& element : array) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

void ArrayProcessor::ReplaceMinMaxWithAverage(std::vector<int>& array,
    int min_value,
    int max_value,
    double average) {
    const int replacement = static_cast<int>(average);
    for (auto& element : array) {
        if (element == min_value || element == max_value) {
            element = replacement;
        }
    }
}

bool InputValidator::IsValidSize(int size) {
    return size >= kMinArraySize && size <= kMaxArraySize;
}

bool InputValidator::IsValidArrayElement(int value) {
    return value >= std::numeric_limits<int>::min() &&
        value <= std::numeric_limits<int>::max();
}