#ifndef AVERAGE_WRAPPER_H
#define AVERAGE_WRAPPER_H

#include <windows.h>
#include <vector>

class AverageWrapper {
public:
    struct Result {
        double value{ 0.0 };
        bool success{ false };
    };

    static DWORD WINAPI ThreadFunction(LPVOID parameter);
    static Result Calculate(const std::vector<int>& data);

private:
    AverageWrapper() = delete;
    ~AverageWrapper() = delete;

    AverageWrapper(const AverageWrapper&) = delete;
    AverageWrapper& operator=(const AverageWrapper&) = delete;

    static constexpr DWORD kDelayMs = 12;
};

#endif