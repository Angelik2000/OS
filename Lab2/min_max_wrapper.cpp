#include "min_max_wrapper.h"
#include <iostream>

// ќбъ€вление глобальных переменных
extern MinMaxWrapper::Result g_min_max_result;

DWORD WINAPI MinMaxWrapper::ThreadFunction(LPVOID parameter) {
    const auto* data = static_cast<const std::vector<int>*>(parameter);
    if (data == nullptr || data->empty()) {
        return 1;
    }

    try {
        g_min_max_result = Calculate(*data); // —охран€ем результат в глобальную переменную
        if (g_min_max_result.success) {
            std::cout << "Min element: " << g_min_max_result.min_value << std::endl;
            std::cout << "Max element: " << g_min_max_result.max_value << std::endl;
        }
        return g_min_max_result.success ? 0 : 1;
    }
    catch (...) {
        return 1;
    }
}

MinMaxWrapper::Result MinMaxWrapper::Calculate(const std::vector<int>& data) {
    Result result;

    if (data.empty()) {
        result.success = false;
        return result;
    }

    result.min_value = data[0];
    result.max_value = data[0];

    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i] < result.min_value) {
            result.min_value = data[i];
        }
        if (data[i] > result.max_value) {
            result.max_value = data[i];
        }
        Sleep(kDelayMs);
    }

    result.success = true;
    return result;
}