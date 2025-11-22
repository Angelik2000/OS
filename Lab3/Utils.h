#pragma once
#include <windows.h>
#include <memory>
#include <stdexcept>

namespace Utils {
    struct HandleDeleter {
        void operator()(HANDLE h) const {
            if (h != nullptr && h != INVALID_HANDLE_VALUE) {
                CloseHandle(h);
            }
        }
    };

    using ScopedHandle = std::unique_ptr<void, HandleDeleter>;

    inline ScopedHandle MakeScopedHandle(HANDLE h) {
        if (h == nullptr || h == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("Failed to create valid handle.");
        }
        return ScopedHandle(h);
    }

    class CriticalSectionGuard {
    public:
        explicit CriticalSectionGuard(CRITICAL_SECTION& cs) : m_cs(cs) {
            EnterCriticalSection(&m_cs);
        }
        ~CriticalSectionGuard() {
            LeaveCriticalSection(&m_cs);
        }
        CriticalSectionGuard(const CriticalSectionGuard&) = delete;
        CriticalSectionGuard& operator=(const CriticalSectionGuard&) = delete;
    private:
        CRITICAL_SECTION& m_cs;
    };
}