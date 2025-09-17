#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include "employee.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Error: Invalid number of arguments." << std::endl;
        std::cerr << "Usage: Creator.exe <filename> <record_count>" << std::endl;
        return 1;
    }

    const char* fileName = argv[1];
    int recordCount = 0;
    try {
        recordCount = std::stoi(argv[2]);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: Invalid record count. " << e.what() << std::endl;
        return 1;
    }

    std::ofstream outputFile(fileName, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Error: Could not open file for writing. GetLastError: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "Enter " << recordCount << " employee records:" << std::endl;

    for (int i = 0; i < recordCount; ++i) {
        Employee emp;
        std::string inputName;

        std::cout << "\nRecord " << i + 1 << ":" << std::endl;
        std::cout << "Enter employee ID: ";
        std::cin >> emp.num;

        std::cout << "Enter employee name (up to " << NAME_MAX_LENGTH - 1 << " chars): ";
        std::cin >> inputName;

        strncpy_s(emp.name, inputName.c_str(), NAME_MAX_LENGTH - 1);
        emp.name[NAME_MAX_LENGTH - 1] = '\0';

        std::cout << "Enter hours worked: ";
        std::cin >> emp.hours;

        outputFile.write(reinterpret_cast<const char*>(&emp), sizeof(Employee));
    }

    outputFile.close();
    std::cout << "\nSuccessfully created binary file '" << fileName << "' with " << recordCount << " records." << std::endl;

    return 0;
}