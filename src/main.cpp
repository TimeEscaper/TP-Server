#include <iostream>
#include <thread>
#include "../include/server/Server.h"

#define ROOT_PATH "/home/sibirsky/static"
#define SERVER_PORT 3490

int main(int argc, char* argv[])
{
    try {
        Server server(SERVER_PORT, ROOT_PATH);
        server.start();
    } catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
