#ifndef TP_SERVER_UTILS_H
#define TP_SERVER_UTILS_H

#include <iostream>
#include <string>
#include <magic.h>
#include <cstring>

namespace utils {

    inline void log(std::string message) { std::cout << message << std::endl; }

    std::string getMimeType(const std::string &filePath);
}

#endif //TP_SERVER_UTILS_H
