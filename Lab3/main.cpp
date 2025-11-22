#include "Orchestrator.h"
#include <iostream>

int main() {
    try {
        Orchestrator app;
        app.Run();
    }
    catch (const std::exception& e) {
        std::cerr << "Critical Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}