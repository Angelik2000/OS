#include "ServerProcess.h"
#include "Employee.h"
#include "FileController.h"
#include "SyncManager.h"
#include "Utils.h"
#include <iostream>
#include <vector>
#include <thread>
#include <windows.h>
#include <string>

// Global controllers (simplified for this architecture)
static FileController fileCtrl;
static SyncManager syncMgr;

void clientHandler(HANDLE rawPipe) {
    HandleGuard pipe(rawPipe); 

    if (!ConnectNamedPipe(pipe.get(), NULL)) {
        if (GetLastError() != ERROR_PIPE_CONNECTED) {
            std::cerr << "Error connecting to client pipe.\n";
            return;
        }
    }

    Command cmd;
    DWORD bytesRead, bytesWritten;
    int currentID = -1;
    bool isWriter = false;

    while (true) {
        if (!ReadFile(pipe.get(), &cmd, sizeof(cmd), &bytesRead, NULL) || bytesRead == 0) {
            break; 
        }

        if (cmd == Command::EXIT_CLIENT) break;

        if (cmd == Command::READ_REQUEST || cmd == Command::MODIFY_REQUEST) {
            int id;
            if (!ReadFile(pipe.get(), &id, sizeof(int), &bytesRead, NULL)) break;

            isWriter = (cmd == Command::MODIFY_REQUEST);

            if (isWriter) syncMgr.lockWriter(id);
            else syncMgr.lockReader(id);

            currentID = id;

           
            Response resp;
            resp.success = fileCtrl.findRecord(id, resp.record);

            if (resp.success) {
                strcpy_s(resp.message, "OK");
            }
            else {
                strcpy_s(resp.message, "Record not found");
               
                if (isWriter) syncMgr.unlockWriter(id);
                else syncMgr.unlockReader(id);
                currentID = -1;
            }

            WriteFile(pipe.get(), &resp, sizeof(Response), &bytesWritten, NULL);
        }
        else if (cmd == Command::SEND_MODIFIED) {
            Employee emp;
            if (ReadFile(pipe.get(), &emp, sizeof(Employee), &bytesRead, NULL)) {
                fileCtrl.updateRecord(emp);
            }
        }
        else if (cmd == Command::FINISH_WORK) {
            if (currentID != -1) {
                if (isWriter) syncMgr.unlockWriter(currentID);
                else syncMgr.unlockReader(currentID);
                currentID = -1;
            }
        }
    }

    if (currentID != -1) {
        if (isWriter) syncMgr.unlockWriter(currentID);
        else syncMgr.unlockReader(currentID);
    }

    FlushFileBuffers(pipe.get());
    DisconnectNamedPipe(pipe.get());
}

void ServerProcess::run() {
    try {
        fileCtrl.createFileFromConsole();
        fileCtrl.printFileContent();

        int clientCount;
        std::cout << "Enter number of client processes: ";
        std::cin >> clientCount;

        char selfPath[MAX_PATH];
        GetModuleFileNameA(NULL, selfPath, MAX_PATH);
        std::string cmdLine = std::string("\"") + selfPath + "\" client";

        std::vector<PROCESS_INFORMATION> processes;

        for (int i = 0; i < clientCount; ++i) {
            STARTUPINFOA si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));

            if (CreateProcessA(NULL, const_cast<char*>(cmdLine.c_str()),
                NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
                processes.push_back(pi);
            }
            else {
                std::cerr << "Failed to start client process. Error: " << GetLastError() << "\n";
            }
        }

        std::vector<std::thread> threads;
        for (int i = 0; i < clientCount; ++i) {
            HANDLE hPipe = CreateNamedPipeA(
                PIPE_NAME,
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                PIPE_UNLIMITED_INSTANCES,
                PIPE_BUFFER_SIZE,
                PIPE_BUFFER_SIZE,
                0, NULL
            );

            if (hPipe == INVALID_HANDLE_VALUE) {
                std::cerr << "CreateNamedPipe failed.\n";
                continue;
            }

            threads.emplace_back(clientHandler, hPipe);
        }

        for (auto& t : threads) {
            if (t.joinable()) t.join();
        }

        for (auto& pi : processes) {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }

        std::cout << "\nAll clients finished.\n";
        fileCtrl.printFileContent();

        std::cout << "Press Enter to exit server...";
        std::cin.ignore();
        std::cin.get();

    }
    catch (const std::exception& e) {
        std::cerr << "Server Exception: " << e.what() << "\n";
    }
}