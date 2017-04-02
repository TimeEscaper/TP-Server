#include <iostream>
#include "../include/server/Server.h"

#define ROOT_PATH "/home/sibirsky/static"

int main(int argc, char* argv[])
{
    Server server(3490, ROOT_PATH);
    server.start();
}
