#include "Marker.h"
#include <iostream>
#include <random>
#include <cstdio>

Marker::Marker(int id, SharedData& sharedData, HANDLE startSignal, HANDLE stopSignal)
    : m_id(id), m_data(sharedData), m_hStartSignal(startSignal), m_hStopSignal(stopSignal)
{
    HANDLE hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    m_hCantContinueEvent = Utils::MakeScopedHandle(hEvent);
}

HANDLE Marker::GetCantContinueEvent() const {
    return m_hCantContinueEvent.get();
}

void Marker::Run() {
    WaitForSingleObject(m_hStartSignal, INFINITE);

    std::mt19937 rng(static_cast<unsigned int>(m_id));
    std::uniform_int_distribution<size_t> dist(0, m_data.array.size() - 1);

    while (true) {
        size_t targetIndex = dist(rng);
        bool markedSuccessfully = false;

        {
            Utils::CriticalSectionGuard lock(m_data.csArrayAccess);
            if (m_data.array[targetIndex] == 0) {
                Sleep(kSleepDurationMs);
                m_data.array[targetIndex] = m_id;
                Sleep(kSleepDurationMs);
                markedSuccessfully = true;
            }
        }

        if (markedSuccessfully) {
            continue;
        }

        printf("Thread %d: Cannot mark index %zu.\n", m_id, targetIndex);

        SetEvent(m_hCantContinueEvent.get());

        HANDLE waitHandles[] = { m_hStartSignal, m_hStopSignal };
        DWORD result = WaitForMultipleObjects(2, waitHandles, FALSE, INFINITE);

        if (result == WAIT_OBJECT_0 + 1) {
            ClearMyMarks();
            return;
        }

        ResetEvent(m_hCantContinueEvent.get());
    }
}

void Marker::ClearMyMarks() {
    Utils::CriticalSectionGuard lock(m_data.csArrayAccess);
    for (int& val : m_data.array) {
        if (val == m_id) {
            val = 0;
        }
    }
}