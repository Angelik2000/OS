#include "common/IPCQueue.hpp"
#include <stdexcept>
#include <vector>
#include <algorithm> 
#include <cstring> 
#include <windows.h> 

namespace lab::ipc {
    IPCQueue::IPCQueue(const std::wstring& fileName, size_t capacity)
        : m_fileName(fileName) {
        InitializeResources(fileName, capacity, true);
    }

    IPCQueue::IPCQueue(const std::wstring& fileName)
        : m_fileName(fileName) {
        InitializeResources(fileName, 0, false);
    }

    void IPCQueue::InitializeResources(const std::wstring& fileName, size_t capacity, bool isCreator) {
        if (isCreator) {
            m_fileHandle = utils::make_scoped_handle(
                CreateFileW(fileName.c_str(), GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL, nullptr));

            if (!m_fileHandle) throw std::runtime_error("Failed to create file.");

            QueueHeader header = {};
            header.capacity = capacity;

            DWORD written = 0;
            WriteFile(m_fileHandle.get(), &header, sizeof(header), &written, nullptr);

            LARGE_INTEGER fileSize = {};
            fileSize.QuadPart = sizeof(QueueHeader) + (capacity * sizeof(Message));
            SetFilePointerEx(m_fileHandle.get(), fileSize, nullptr, FILE_BEGIN);
            SetEndOfFile(m_fileHandle.get());
        }
        else {
            m_fileHandle = utils::make_scoped_handle(
                CreateFileW(fileName.c_str(), GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL, nullptr));
            if (!m_fileHandle) throw std::runtime_error("Failed to open file.");
        }

        m_mutex = utils::make_scoped_handle(
            CreateMutexW(nullptr, FALSE, GetMutexName(fileName).c_str()));

        long initialEmpty = isCreator ? static_cast<long>(capacity) : 0;

        if (isCreator) {
            m_semEmpty = utils::make_scoped_handle(
                CreateSemaphoreW(nullptr, initialEmpty, static_cast<long>(capacity), GetSemEmptyName(fileName).c_str()));

            m_semFull = utils::make_scoped_handle(
                CreateSemaphoreW(nullptr, 0, static_cast<long>(capacity), GetSemFullName(fileName).c_str()));
        }
        else {
            m_semEmpty = utils::make_scoped_handle(
                OpenSemaphoreW(SEMAPHORE_ALL_ACCESS, FALSE, GetSemEmptyName(fileName).c_str()));

            m_semFull = utils::make_scoped_handle(
                OpenSemaphoreW(SEMAPHORE_ALL_ACCESS, FALSE, GetSemFullName(fileName).c_str()));
        }

        if (!m_mutex || !m_semEmpty || !m_semFull) {
            throw std::runtime_error("Failed to init sync primitives.");
        }
    }

    void IPCQueue::Push(const std::string& msgStr) {
        WaitForSingleObject(m_semEmpty.get(), INFINITE);
        WaitForSingleObject(m_mutex.get(), INFINITE);

        try {
            QueueHeader header = {};
            DWORD rwBytes = 0;

            SetFilePointer(m_fileHandle.get(), 0, nullptr, FILE_BEGIN);
            ReadFile(m_fileHandle.get(), &header, sizeof(header), &rwBytes, nullptr);

            Message msg{};
            size_t lengthCopy = std::min(msgStr.length(), MAX_MESSAGE_LEN);
            std::memcpy(msg.text, msgStr.c_str(), lengthCopy);

            long offset = sizeof(QueueHeader) + (static_cast<long>(header.tail) * sizeof(Message));
            SetFilePointer(m_fileHandle.get(), offset, nullptr, FILE_BEGIN);
            WriteFile(m_fileHandle.get(), &msg, sizeof(Message), &rwBytes, nullptr);

            header.tail = (header.tail + 1) % header.capacity;
            header.count++;
            SetFilePointer(m_fileHandle.get(), 0, nullptr, FILE_BEGIN);
            WriteFile(m_fileHandle.get(), &header, sizeof(header), &rwBytes, nullptr);

        }
        catch (...) {
            ReleaseMutex(m_mutex.get());
            throw;
        }

        ReleaseMutex(m_mutex.get());
        ReleaseSemaphore(m_semFull.get(), 1, nullptr);
    }

    std::string IPCQueue::Pop() {
        WaitForSingleObject(m_semFull.get(), INFINITE);
        WaitForSingleObject(m_mutex.get(), INFINITE);

        std::string result;
        try {
            QueueHeader header = {};
            DWORD rwBytes = 0;

            SetFilePointer(m_fileHandle.get(), 0, nullptr, FILE_BEGIN);
            ReadFile(m_fileHandle.get(), &header, sizeof(header), &rwBytes, nullptr);

            Message msg{};
            long offset = sizeof(QueueHeader) + (static_cast<long>(header.head) * sizeof(Message));
            SetFilePointer(m_fileHandle.get(), offset, nullptr, FILE_BEGIN);
            ReadFile(m_fileHandle.get(), &msg, sizeof(Message), &rwBytes, nullptr);

            result = std::string(msg.text, strnlen(msg.text, MAX_MESSAGE_LEN));

            header.head = (header.head + 1) % header.capacity;
            header.count--;
            SetFilePointer(m_fileHandle.get(), 0, nullptr, FILE_BEGIN);
            WriteFile(m_fileHandle.get(), &header, sizeof(header), &rwBytes, nullptr);

        }
        catch (...) {
            ReleaseMutex(m_mutex.get());
            throw;
        }

        ReleaseMutex(m_mutex.get());
        ReleaseSemaphore(m_semEmpty.get(), 1, nullptr);

        return result;
    }

    std::wstring IPCQueue::GetMutexName(const std::wstring& f) const { return f + L"_MUTEX"; }
    std::wstring IPCQueue::GetSemFullName(const std::wstring& f) const { return f + L"_SEM_FULL"; }
    std::wstring IPCQueue::GetSemEmptyName(const std::wstring& f) const { return f + L"_SEM_EMPTY"; }
}