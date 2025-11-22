#pragma once
#include "AutoHandle.hpp"
#include <string>
#include <vector>

namespace lab::ipc {

    constexpr size_t MAX_MESSAGE_LEN = 20;

    struct Message {
        char text[MAX_MESSAGE_LEN];
    };

    struct QueueHeader {
        size_t head;
        size_t tail;
        size_t capacity;
        size_t count;
    };

    class IPCQueue {
    public:
        IPCQueue(const std::wstring& fileName, size_t capacity);
        IPCQueue(const std::wstring& fileName);

        IPCQueue(const IPCQueue&) = delete;
        IPCQueue& operator=(const IPCQueue&) = delete;

        ~IPCQueue() = default;

        void Push(const std::string& msg);
        std::string Pop();

    private:
        void InitializeResources(const std::wstring& fileName, size_t capacity, bool isCreator);
        std::wstring GetMutexName(const std::wstring& fileName) const;
        std::wstring GetSemFullName(const std::wstring& fileName) const;
        std::wstring GetSemEmptyName(const std::wstring& fileName) const;

    private:
        std::wstring m_fileName;
        utils::ScopedHandle m_fileHandle;
        utils::ScopedHandle m_mutex;
        utils::ScopedHandle m_semFull;
        utils::ScopedHandle m_semEmpty;
    };
}