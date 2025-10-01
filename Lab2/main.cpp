#include <iostream>
#include <vector>
#include <limits>

#include "core.h"
#include "min_max_wrapper.h"
#include "average_wrapper.h"
#include "thread_utils.h"

// Глобальные переменные для хранения результатов потоков
MinMaxWrapper::Result g_min_max_result;
AverageWrapper::Result g_average_result;

int main() {
    try {
        int size = 0;

        while (true) {
            std::cout << "Enter array size: ";
            if (std::cin >> size) {
                if (InputValidator::IsValidSize(size)) {
                    break;
                }
                else {
                    std::cout << "Error: Array size must be between "
                        << InputValidator::kMinArraySize << " and "
                        << InputValidator::kMaxArraySize << std::endl;
                }
            }
            else {
                std::cout << "Invalid input. Please enter an integer." << std::endl;
                std::cin.clear();
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            }
        }

        std::vector<int> array(static_cast<size_t>(size));
        ArrayProcessor::InitializeArray(array);

        std::cout << "Original array: ";
        ArrayProcessor::PrintArray(array);

        HandleGuard threads[2];

        threads[0].reset(CreateThread(
            nullptr,
            0,
            MinMaxWrapper::ThreadFunction,
            &array,
            0,
            nullptr
        ));

        CheckThreadResult(static_cast<bool>(threads[0]), "Create min_max thread");

        threads[1].reset(CreateThread(
            nullptr,
            0,
            AverageWrapper::ThreadFunction,
            &array,
            0,
            nullptr
        ));

        CheckThreadResult(static_cast<bool>(threads[1]), "Create average thread");

        std::cout << "Waiting for threads to complete..." << std::endl;

        const HANDLE thread_handles[] = { threads[0].get(), threads[1].get() };
        const DWORD wait_result = WaitForMultipleObjects(
            2,
            thread_handles,
            TRUE,
            INFINITE
        );

        CheckThreadResult(wait_result != WAIT_FAILED, "WaitForMultipleObjects");

        // Используем результаты из глобальных переменных
        if (g_min_max_result.success && g_average_result.success) {
            ArrayProcessor::ReplaceMinMaxWithAverage(
                array,
                g_min_max_result.min_value,
                g_min_max_result.max_value,
                g_average_result.value
            );

            std::cout << "Modified array: ";
            ArrayProcessor::PrintArray(array);
        }
        else {
            std::cerr << "Error: Failed to calculate min/max or average values" << std::endl;
            return 1;
        }

        std::cout << "Program completed successfully" << std::endl;
        return 0;
    }
    catch (const ThreadException& e) {
        std::cerr << "Thread error: " << e.what() << " (code: " << e.code() << ")" << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }
}