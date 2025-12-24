#include "ClientProcess.h"
#include "Employee.h"
#include "Utils.h"
#include <iostream>
#include <conio.h> 

void ClientProcess::run() {
    
    if (!WaitNamedPipeA(PIPE_NAME, NMPWAIT_WAIT_FOREVER)) {
        std::cerr << "Failed to wait for pipe.\n";
        system("pause");
        return;
    }

    HANDLE hPipe = CreateFileA(
        PIPE_NAME, GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, 0, NULL
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Connection failed. Error: " << GetLastError() << "\n";
        system("pause");
        return;
    }

    HandleGuard pipe(hPipe);
    DWORD bytesWritten, bytesRead;
    bool active = true;

    while (active) {
        system("cls");
        std::cout << "=== CLIENT ===\n";
        std::cout << "1. Modify Record\n";
        std::cout << "2. Read Record\n";
        std::cout << "3. Exit\n";
        std::cout << "> ";

        int choice;
        std::cin >> choice;

        if (choice == 3) {
            Command cmd = Command::EXIT_CLIENT;
            WriteFile(pipe.get(), &cmd, sizeof(cmd), &bytesWritten, NULL);
            active = false;
            continue;
        }

        if (choice != 1 && choice != 2) continue;

        int id;
        std::cout << "Enter Employee ID: ";
        std::cin >> id;

        Command cmd = (choice == 1) ? Command::MODIFY_REQUEST : Command::READ_REQUEST;
        if (!WriteFile(pipe.get(), &cmd, sizeof(cmd), &bytesWritten, NULL)) break;
        if (!WriteFile(pipe.get(), &id, sizeof(id), &bytesWritten, NULL)) break;

        std::cout << "Request sent. Waiting for access...\n";

        Response resp;
        if (!ReadFile(pipe.get(), &resp, sizeof(resp), &bytesRead, NULL)) {
            std::cerr << "Server disconnected.\n";
            break;
        }

        if (resp.success) {
            std::cout << "\n--- RECORD ---\n";
            std::cout << "ID: " << resp.record.num << "\n";
            std::cout << "Name: " << resp.record.name << "\n";
            std::cout << "Hours: " << resp.record.hours << "\n";

            if (choice == 1) {
                std::cout << "\n--- MODIFY ---\n";
                std::cout << "New Name: "; std::cin >> resp.record.name;
                std::cout << "New Hours: "; std::cin >> resp.record.hours;

                Command updateCmd = Command::SEND_MODIFIED;
                WriteFile(pipe.get(), &updateCmd, sizeof(updateCmd), &bytesWritten, NULL);
                WriteFile(pipe.get(), &resp.record, sizeof(Employee), &bytesWritten, NULL);
                std::cout << "Data updated.\n";
            }

            std::cout << "\nPress key to finish session...";
            _getch();

            Command finCmd = Command::FINISH_WORK;
            WriteFile(pipe.get(), &finCmd, sizeof(finCmd), &bytesWritten, NULL);
        }
        else {
            std::cout << "Server Message: " << resp.message << "\n";
            std::cout << "Press key...";
            _getch();
        }
    }
}