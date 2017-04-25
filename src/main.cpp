#include <iostream>
#include <thread>
#include "../include/server/Server.h"
#include "../include/helpers/utils.h"

#define ROOT_PATH "/home/sibirsky/static"
#define SERVER_PORT 3490

Server server(SERVER_PORT, ROOT_PATH);

void *serverThreadWork(void *arg) {
    try {
        server.start();
    } catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
    }
}

int main(int argc, char* argv[])
{
    /*try {
        std::thread serverThread(serverThreadWork);
        getchar();
        server.stop();
        serverThread.join();
    } catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
    } */
    pthread_t pthread;
    int err;
    err = pthread_create(&pthread, NULL, serverThreadWork, NULL);
    if (err != 0) {
        utils::log("Error creating server thread!");
        return 0;
    }
    utils::log("Server thread created!");
    getchar();
    err = pthread_cancel(pthread);
    if (err != 0) {
        utils::log("Error cancelling thread!");
        return 0;
    }
    utils::log("Server thread cancelled!");
    err = pthread_detach(pthread);
    if (err != 0) {
        utils::log("Error detaching server!");
        return 0;
    }
    utils::log("Server thread detached!");

    return 0;
}
