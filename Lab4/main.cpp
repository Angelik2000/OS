#include "common/IPCQueue.hpp"
#include <iostream>
#include <string>
#include <limits>
#include <windows.h>

int main(int argc, char* argv[]) {
    try {
        if (argc < 3) return 1;

        std::string filenameOpenStr = argv[1];
        std::string eventNameOpenStr = argv[2];

        std::wstring filename(filenameOpenStr.begin(), filenameOpenStr.end());
        std::wstring eventName(eventNameOpenStr.begin(), eventNameOpenStr.end());

        lab::ipc::IPCQueue queue(filename);

        HANDLE hEvent = OpenEventW(EVENT_MODIFY_STATE, FALSE, eventName.c_str());
        if (hEvent) {
            SetEvent(hEvent);
            CloseHandle(hEvent);
        }

        std::cout << "Sender Started.\n";

        while (true) {
            std::cout << "\n1. Send\n2. Exit\n> ";

            int ch = 0;
            std::cin >> ch;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (ch == 2) break;

            if (ch == 1) {
                std::cout << "Msg: ";
                std::string msg;
                std::cin >> msg;
                queue.Push(msg);
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}