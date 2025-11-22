#pragma once
#include "SharedData.h"
#include <memory>

class Orchestrator {
public:
    void Run();

private:
    static DWORD WINAPI ThreadProc(LPVOID lpParam);
    void PrintArray(const SharedData& data);
};