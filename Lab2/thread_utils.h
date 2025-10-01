#ifndef THREAD_UTILS_H
#define THREAD_UTILS_H

#include <windows.h>
#include <string>
#include <system_error>

class HandleGuard {
public:
    explicit HandleGuard(HANDLE handle = nullptr) noexcept : handle_(handle) {}

    ~HandleGuard() {
        if (handle_ != nullptr && handle_ != INVALID_HANDLE_VALUE) {
            CloseHandle(handle_);
        }
    }

    HandleGuard(HandleGuard&& other) noexcept : handle_(other.handle_) {
        other.handle_ = nullptr;
    }

    HandleGuard& operator=(HandleGuard&& other) noexcept {
        if (this != &other) {
            reset();
            handle_ = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

    HandleGuard(const HandleGuard&) = delete;
    HandleGuard& operator=(const HandleGuard&) = delete;

    explicit operator bool() const noexcept {
        return handle_ != nullptr && handle_ != INVALID_HANDLE_VALUE;
    }

    HANDLE get() const noexcept { return handle_; }

    void reset(HANDLE handle = nullptr) noexcept {
        if (handle_ != nullptr && handle_ != INVALID_HANDLE_VALUE) {
            CloseHandle(handle_);
        }
        handle_ = handle;
    }

    HANDLE release() noexcept {
        HANDLE temp = handle_;
        handle_ = nullptr;
        return temp;
    }

private:
    HANDLE handle_;
};

class ThreadException : public std::system_error {
public:
    ThreadException(const std::string& message, DWORD error_code)
        : std::system_error(error_code, std::system_category(), message) {}
};

// Убрана inline реализация здесь - она будет в thread_utils.cpp
void CheckThreadResult(bool success, const std::string& operation);

#endif