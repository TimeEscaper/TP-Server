#include <iostream>
#include "../include/server/Server.h"

int main(int argc, char* argv[])
{
    Server server(3490);
    server.start();
}
