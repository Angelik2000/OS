#include "thread_utils.h"

void CheckThreadResult(bool success, const std::string& operation) {
    if (!success) {
        throw ThreadException(operation, GetLastError());
    }
}