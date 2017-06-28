#ifndef TP_SERVER_UTILS_H
#define TP_SERVER_UTILS_H

#include <iostream>
#include <string>
#include <cstring>
#include <map>

#define DEBUG true
#define LOGGING true

namespace log {
    inline void debug(std::string message) { if (DEBUG) { std::cout << message << std::endl; }}
    inline void log(std::string message) { if (LOGGING) { std::cout << message << std::endl; }}
}

#endif //TP_SERVER_UTILS_H
