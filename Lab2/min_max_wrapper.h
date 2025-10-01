#ifndef MIN_MAX_WRAPPER_H
#define MIN_MAX_WRAPPER_H

#include <windows.h>
#include <vector>

class MinMaxWrapper {
public:
    struct Result {
        int min_value{ 0 };
        int max_value{ 0 };
        bool success{ false };
    };

    static DWORD WINAPI ThreadFunction(LPVOID parameter);
    static Result Calculate(const std::vector<int>& data);

private:
    MinMaxWrapper() = delete;
    ~MinMaxWrapper() = delete;

    MinMaxWrapper(const MinMaxWrapper&) = delete;
    MinMaxWrapper& operator=(const MinMaxWrapper&) = delete;

    static constexpr DWORD kDelayMs = 7;
};

#endif