#pragma once
#include <vector>
#include <windows.h>

struct SharedData {
    std::vector<int> array;
    CRITICAL_SECTION csArrayAccess;

    SharedData(size_t size) : array(size, 0) {
        InitializeCriticalSection(&csArrayAccess);
    }

    ~SharedData() {
        DeleteCriticalSection(&csArrayAccess);
    }

    SharedData(const SharedData&) = delete;
    SharedData& operator=(const SharedData&) = delete;
};