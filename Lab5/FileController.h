#pragma once
#include "Employee.h"
#include <mutex>
#include <vector>

class FileController {
private:
    std::mutex fileMutex; 

public:
    void createFileFromConsole();
    void printFileContent();
    bool findRecord(int id, Employee& outEmp);
    void updateRecord(const Employee& emp);
};