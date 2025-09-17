#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <windows.h>
#include "employee.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Error: Invalid number of arguments." << std::endl;
        std::cerr << "Usage: Reporter.exe <binary_filename> <report_filename> <hourly_rate>" << std::endl;
        return 1;
    }

    const char* binaryFileName = argv[1];
    const char* reportFileName = argv[2];
    double hourlyRate = 0.0;
    try {
        hourlyRate = std::stod(argv[3]);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: Invalid hourly rate. " << e.what() << std::endl;
        return 1;
    }

    std::ifstream inputFile(binaryFileName, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error: Could not open binary file '" << binaryFileName << "'. GetLastError: " << GetLastError() << std::endl;
        return 1;
    }

    std::ofstream reportFile(reportFileName);
    if (!reportFile) {
        std::cerr << "Error: Could not open report file '" << reportFileName << "'. GetLastError: " << GetLastError() << std::endl;
        inputFile.close();
        return 1;
    }

    reportFile << "Report for file: " << binaryFileName << std::endl;
    reportFile << "--------------------------------------" << std::endl;
    reportFile << std::left << std::setw(15) << "Employee ID"
        << std::setw(15) << "Name"
        << std::setw(15) << "Hours"
        << std::setw(15) << "Salary" << std::endl;

    Employee emp;
    while (inputFile.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
        double salary = emp.hours * hourlyRate;
        reportFile << std::left << std::setw(15) << emp.num
            << std::setw(15) << emp.name
            << std::setw(15) << std::fixed << std::setprecision(2) << emp.hours
            << std::setw(15) << std::fixed << std::setprecision(2) << salary << std::endl;
    }

    inputFile.close();
    reportFile.close();

    std::cout << "\nSuccessfully created report file '" << reportFileName << "'." << std::endl;

    return 0;
}