#pragma once

#include <string>

const int NAME_MAX_LENGTH = 10;

struct Employee {
    int num;
    char name[NAME_MAX_LENGTH];
    double hours;
};