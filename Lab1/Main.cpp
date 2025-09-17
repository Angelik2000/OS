#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <vector>
#include <locale>
#include "employee.h"

void readFileContents(const std::string& fileName) {
    std::cout << "--- Contents of '" << fileName << "' ---" << std::endl;
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Error: Could not open file for reading: " << fileName << std::endl;
        return;
    }
    std::cout << file.rdbuf();
    std::cout << "\n-----------------------------------------" << std::endl;
    file.close();
}

std::wstring GetCurrentPath() {
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);
    std::wstring currentPath = path;
    currentPath = currentPath.substr(0, currentPath.find_last_of(L"\\/"));
    return currentPath;
}

int main() {
    std::locale::global(std::locale(""));

    std::string binaryFileName, reportFileName;
    int recordCount;
    double hourlyRate;

    std::cout << "--- Creator Process Setup ---" << std::endl;
    std::cout << "Enter the name for the binary file: ";
    std::cin >> binaryFileName;

    std::cout << "Enter the number of records: ";
    std::cin >> recordCount;

    std::wstring mainPath = GetCurrentPath();
    std::wstring creatorExePath = mainPath + L"\\Creator.exe";

   
    std::wstring wCreatorCmd = L"\"" + creatorExePath + L"\" " + std::wstring(binaryFileName.begin(), binaryFileName.end()) + L" " + std::to_wstring(recordCount);

    std::vector<wchar_t> creatorCmdLine(wCreatorCmd.begin(), wCreatorCmd.end());
    creatorCmdLine.push_back(L'\0');

    STARTUPINFO siCreator;
    PROCESS_INFORMATION piCreator;

    ZeroMemory(&siCreator, sizeof(siCreator));
    siCreator.cb = sizeof(siCreator);
    ZeroMemory(&piCreator, sizeof(piCreator));

    std::wcout << L"\nStarting Creator.exe..." << std::endl;

    if (!CreateProcessW(
        NULL,
        creatorCmdLine.data(),
        NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE,
        NULL, NULL, &siCreator, &piCreator)) {
        std::cerr << "Error: CreateProcessW failed for Creator. GetLastError: " << GetLastError() << std::endl;
        return 1;
    }

    WaitForSingleObject(piCreator.hProcess, INFINITE);
    std::cout << "Creator.exe has finished its work." << std::endl;

    CloseHandle(piCreator.hProcess);
    CloseHandle(piCreator.hThread);

    std::cout << "\nReading and displaying the content of the created binary file:" << std::endl;
    std::ifstream binFile(binaryFileName, std::ios::binary);
    if (binFile) {
        Employee emp;
        while (binFile.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
            std::cout << "ID: " << emp.num << ", Name: " << emp.name << ", Hours: " << emp.hours << std::endl;
        }
        binFile.close();
    }
    else {
        std::cerr << "Error: Failed to open the binary file for reading." << std::endl;
    }

    std::cout << "\n--- Reporter Process Setup ---" << std::endl;
    std::cout << "Enter the name for the report file: ";
    std::cin >> reportFileName;

    std::cout << "Enter the hourly rate: ";
    std::cin >> hourlyRate;

    std::wstring reporterExePath = mainPath + L"\\Reporter.exe";
    std::wstring wReporterCmd = L"\"" + reporterExePath + L"\" " + std::wstring(binaryFileName.begin(), binaryFileName.end()) + L" " + std::wstring(reportFileName.begin(), reportFileName.end()) + L" " + std::to_wstring(hourlyRate);

    std::vector<wchar_t> reporterCmdLine(wReporterCmd.begin(), wReporterCmd.end());
    reporterCmdLine.push_back(L'\0');

    STARTUPINFO siReporter;
    PROCESS_INFORMATION piReporter;

    ZeroMemory(&siReporter, sizeof(siReporter));
    siReporter.cb = sizeof(siReporter);
    ZeroMemory(&piReporter, sizeof(piReporter));

    std::wcout << L"\nStarting Reporter.exe..." << std::endl;

    if (!CreateProcessW(
        NULL,
        reporterCmdLine.data(),
        NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE,
        NULL, NULL, &siReporter, &piReporter)) {
        std::cerr << "Error: CreateProcessW failed for Reporter. GetLastError: " << GetLastError() << std::endl;
        return 1;
    }

    WaitForSingleObject(piReporter.hProcess, INFINITE);
    std::cout << "Reporter.exe has finished its work." << std::endl;

    CloseHandle(piReporter.hProcess);
    CloseHandle(piReporter.hThread);

    std::cout << "\nDisplaying the content of the created report file:" << std::endl;
    readFileContents(reportFileName);

    std::cout << "\nMain program has finished." << std::endl;

    return 0;
}