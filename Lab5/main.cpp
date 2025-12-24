#include <iostream>
#include <cstring>
#include "ServerProcess.h"
#include "ClientProcess.h"

int main(int argc, char* argv[]) {
   
    if (argc > 1 && std::strcmp(argv[1], "client") == 0) {
        ClientProcess client;
        client.run();
    }
    else {
        ServerProcess server;
        server.run();
    }
    return 0;
}