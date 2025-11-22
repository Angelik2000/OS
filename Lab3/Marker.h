#pragma once
#include "SharedData.h"
#include "Utils.h"

class Marker {
public:
    static constexpr int kSleepDurationMs = 5;

    Marker(int id, SharedData& sharedData, HANDLE startSignal, HANDLE stopSignal);

    void Run();
    HANDLE GetCantContinueEvent() const;
    int GetId() const { return m_id; }

private:
    int m_id;
    SharedData& m_data;
    HANDLE m_hStartSignal;
    HANDLE m_hStopSignal;
    Utils::ScopedHandle m_hCantContinueEvent;

    void ClearMyMarks();
};