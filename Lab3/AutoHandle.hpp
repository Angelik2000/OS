#pragma once
#include <windows.h> 
#include <memory>

namespace lab::utils {

    struct HandleDeleter {
        void operator()(HANDLE handle) const {
            if (INVALID_HANDLE_VALUE != handle && nullptr != handle) {
                ::CloseHandle(handle);
            }
        }
    };

    using ScopedHandle = std::unique_ptr<void, HandleDeleter>;

    inline ScopedHandle make_scoped_handle(HANDLE handle) {
        if (INVALID_HANDLE_VALUE == handle || nullptr == handle) {
            return ScopedHandle(nullptr);
        }
        return ScopedHandle(handle);
    }
}