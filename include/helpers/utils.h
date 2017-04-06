#ifndef TP_SERVER_UTILS_H
#define TP_SERVER_UTILS_H

#include <iostream>
#include <string>

namespace utils {

    inline void log(std::string message) { std::cout << message << std::endl; }

    inline char* newString() {
        char* str = new char;
        str[0] = '\0';
        return str;
    }
}

#endif //TP_SERVER_UTILS_H
