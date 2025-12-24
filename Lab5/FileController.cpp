#include "FileController.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

void FileController::createFileFromConsole() {
    std::lock_guard<std::mutex> lock(fileMutex);

    std::cout << "--- Creating Binary File ---\n";
    int count;
    std::cout << "Enter number of employees: ";
    std::cin >> count;

    std::ofstream outFile(FILE_NAME, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Failed to create file: " + std::string(FILE_NAME));
    }

    for (int i = 0; i < count; ++i) {
        Employee e;
        std::cout << "Employee #" << (i + 1) << "\n";
        std::cout << "ID: "; std::cin >> e.num;
        std::cout << "Name: "; std::cin >> e.name;
        std::cout << "Hours: "; std::cin >> e.hours;
        outFile.write(reinterpret_cast<const char*>(&e), sizeof(Employee));
    }
    outFile.close();
    std::cout << "File created successfully.\n";
}

void FileController::printFileContent() {
    std::lock_guard<std::mutex> lock(fileMutex);

    std::ifstream inFile(FILE_NAME, std::ios::binary);
    if (!inFile) return;

    Employee e;
    std::cout << "\n=== File Content ===\n";
    while (inFile.read(reinterpret_cast<char*>(&e), sizeof(Employee))) {
        std::cout << "ID: " << e.num << ", Name: " << e.name << ", Hours: " << e.hours << "\n";
    }
    std::cout << "====================\n";
}

bool FileController::findRecord(int id, Employee& outEmp) {
    std::lock_guard<std::mutex> lock(fileMutex);

    std::ifstream inFile(FILE_NAME, std::ios::binary);
    Employee e;
    while (inFile.read(reinterpret_cast<char*>(&e), sizeof(Employee))) {
        if (e.num == id) {
            outEmp = e;
            return true;
        }
    }
    return false;
}

void FileController::updateRecord(const Employee& emp) {
    std::lock_guard<std::mutex> lock(fileMutex);

    std::fstream file(FILE_NAME, std::ios::in | std::ios::out | std::ios::binary);
    Employee e;
    while (file.read(reinterpret_cast<char*>(&e), sizeof(Employee))) {
        if (e.num == emp.num) {
            
            file.seekp(-static_cast<int>(sizeof(Employee)), std::ios::cur);
            file.write(reinterpret_cast<const char*>(&emp), sizeof(Employee));
            break;
        }
    }
}