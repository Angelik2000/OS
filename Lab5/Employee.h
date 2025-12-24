#pragma once
#include <string>

constexpr char PIPE_NAME[] = "\\\\.\\pipe\\Lab5EmployeePipe";
constexpr char FILE_NAME[] = "employees.bin";
constexpr int PIPE_BUFFER_SIZE = 1024;

struct Employee {
    int num;
    char name[10];
    double hours;
};

enum class Command {
    READ_REQUEST,       
    MODIFY_REQUEST,     
    SEND_MODIFIED,      
    FINISH_WORK,        
    EXIT_CLIENT         
};

struct Response {
    Employee record;
    bool success;
    char message[64];
};