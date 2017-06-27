#include <iostream>
#include <thread>
#include <cstring>
#include "../include/server/Server.h"
#include "../include/logging/utils.h"

#define ROOT_PATH "/home/sibirsky/static"
#define SERVER_PORT 3490
#define DEFAULT_POOL_SIZE 16

void *serverThreadWork(void *arg) {
    try {
        static_cast<Server*>(arg)->start();
    } catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
    }
}

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
        rootPath = ROOT_PATH;
    }
    Server *server = new Server(SERVER_PORT, rootPath, DEFAULT_POOL_SIZE, ncpu);
    pthread_t pthread;
    int err;
    err = pthread_create(&pthread, NULL, serverThreadWork, server);
    if (err != 0) {
        log::log("Error creating server thread!");
        return 0;
    }
    log::log("Server thread created!");
    getchar();
    err = pthread_cancel(pthread);
    if (err != 0) {
        log::log("Error cancelling thread!");
        return 0;
    }
    log::log("Server thread cancelled!");
    err = pthread_detach(pthread);
    if (err != 0) {
        log::log("Error detaching server!");
        return 0;
    }
    log::log("Server thread detached!");
    delete server;

    return 0;
}
