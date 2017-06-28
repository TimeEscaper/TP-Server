#include <iostream>
#include <thread>
#include <cstring>
#include "../include/server/Server.h"
#include "../include/logging/log.h"

#define SERVER_PORT 80

int main(int argc, char* argv[])
{
    std::string rootPath;
    int ncpu = 0;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            i++;
            rootPath = argv[i];
        } else if (strcmp(argv[i], "-c") == 0) {
            i++;
            ncpu = atoi(argv[i]);
        }
    }
    if (rootPath.length() == 0) {
        throw new std::runtime_error("Root directory required!");
    }

    Server server(SERVER_PORT, rootPath, 1, ncpu);
    server.start();

    return 0;
}
