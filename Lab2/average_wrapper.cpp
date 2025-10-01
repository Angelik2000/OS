#include "average_wrapper.h"
#include <iostream>

// ќбъ€вление глобальных переменных
extern AverageWrapper::Result g_average_result;

DWORD WINAPI AverageWrapper::ThreadFunction(LPVOID parameter) {
    const auto* data = static_cast<const std::vector<int>*>(parameter);
    if (data == nullptr || data->empty()) {
        return 1;
    }

    try {
        g_average_result = Calculate(*data); // —охран€ем результат в глобальную переменную
        if (g_average_result.success) {
            std::cout << "Average value: " << g_average_result.value << std::endl;
        }
        return g_average_result.success ? 0 : 1;
    }
    catch (...) {
        return 1;
    }
}

AverageWrapper::Result AverageWrapper::Calculate(const std::vector<int>& data) {
    Result result;

    if (data.empty()) {
        result.success = false;
        return result;
    }

    long long sum = 0;
    for (const auto& element : data) {
        sum += element;
        Sleep(kDelayMs);
    }

    result.value = static_cast<double>(sum) / data.size();
    result.success = true;
    return result;
}