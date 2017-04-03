#include <iostream>
#include <thread>
#include "../include/server/Server.h"

#define ROOT_PATH "/home/sibirsky/static"
#define SERVER_PORT 3490

Server server(SERVER_PORT, ROOT_PATH);

void serverThreadWork() {
    try {
        server.start();
    } catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
    }
}

int main(int argc, char* argv[])
{
    try {
        std::thread serverThread(serverThreadWork);
        getchar();
        server.stop();
        serverThread.join();
    } catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
