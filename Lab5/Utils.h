#pragma once
#include <windows.h>

class HandleGuard {
private:
    HANDLE handle;

public:
    explicit HandleGuard(HANDLE h) : handle(h) {}

    ~HandleGuard() {
        if (handle != INVALID_HANDLE_VALUE && handle != nullptr) {
            CloseHandle(handle);
        }
    }

    HandleGuard(const HandleGuard&) = delete;
    HandleGuard& operator=(const HandleGuard&) = delete;

    HandleGuard(HandleGuard&& other) noexcept : handle(other.handle) {
        other.handle = INVALID_HANDLE_VALUE;
    }

    HANDLE get() const { return handle; }

    bool isValid() const {
        return handle != INVALID_HANDLE_VALUE && handle != nullptr;
    }
};