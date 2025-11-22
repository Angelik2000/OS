#include "Orchestrator.h"
#include "Marker.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>
#include <vector>

DWORD WINAPI Orchestrator::ThreadProc(LPVOID lpParam) {
    auto* marker = static_cast<Marker*>(lpParam);
    marker->Run();
    return 0;
}

void Orchestrator::Run() {
    int arraySize = 0;
    int threadCount = 0;

    std::cout << "Enter array size: ";
    std::cin >> arraySize;
    if (arraySize <= 0) throw std::invalid_argument("Array size must be positive");

    std::cout << "Enter thread count: ";
    std::cin >> threadCount;
    if (threadCount <= 0 || threadCount > 64)
        throw std::invalid_argument("Invalid thread count (Max 64)");

    SharedData sharedData(static_cast<size_t>(arraySize));

    Utils::ScopedHandle hGlobalStart = Utils::MakeScopedHandle(CreateEvent(nullptr, TRUE, FALSE, nullptr));

    std::vector<std::unique_ptr<Marker>> markers;
    std::vector<Utils::ScopedHandle> hThreads;
    std::vector<Utils::ScopedHandle> hStopEvents;
    std::vector<HANDLE> hCantContinueEvents; 

    for (int i = 1; i <= threadCount; ++i) {
        auto hStop = Utils::MakeScopedHandle(CreateEvent(nullptr, FALSE, FALSE, nullptr));

        auto marker = std::make_unique<Marker>(i, sharedData, hGlobalStart.get(), hStop.get());

        hCantContinueEvents.push_back(marker->GetCantContinueEvent());

        DWORD threadId;
        HANDLE rawThread = CreateThread(nullptr, 0, ThreadProc, marker.get(), 0, &threadId);
        hThreads.push_back(Utils::MakeScopedHandle(rawThread));

        hStopEvents.push_back(std::move(hStop));
        markers.push_back(std::move(marker));
    }

    std::cout << "Starting threads..." << std::endl;
    SetEvent(hGlobalStart.get()); 

    Sleep(100);

    ResetEvent(hGlobalStart.get()); 

    int activeThreads = threadCount;
    while (activeThreads > 0) {

        if (!hCantContinueEvents.empty()) {
            WaitForMultipleObjects(static_cast<DWORD>(hCantContinueEvents.size()),
                hCantContinueEvents.data(), TRUE, INFINITE);
        }

        PrintArray(sharedData);

        int idToKill = -1;
        std::cout << "Enter Marker ID to stop: ";
        std::cin >> idToKill;

        auto it = std::find_if(markers.begin(), markers.end(),
            [idToKill](const auto& m) { return m->GetId() == idToKill; });

        if (it != markers.end()) {
            size_t index = std::distance(markers.begin(), it);

            SetEvent(hStopEvents[index].get());

            WaitForSingleObject(hThreads[index].get(), INFINITE);

            PrintArray(sharedData);

            markers.erase(markers.begin() + index);
            hThreads.erase(hThreads.begin() + index);
            hStopEvents.erase(hStopEvents.begin() + index);
            hCantContinueEvents.erase(hCantContinueEvents.begin() + index);

            activeThreads--;
        }
        else {
            std::cout << "Marker not found with ID " << idToKill << ".\n";
        }

        if (activeThreads > 0) {
            SetEvent(hGlobalStart.get()); 
            Sleep(100);                  
            ResetEvent(hGlobalStart.get()); 
        }
    }

    std::cout << "All threads finished.\n";
}

void Orchestrator::PrintArray(const SharedData& data) {
    std::cout << "Array: [ ";
    for (const auto& val : data.array) {
        std::cout << val << " ";
    }
    std::cout << "]\n";
}