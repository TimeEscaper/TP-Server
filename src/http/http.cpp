#include <cstdlib>
#include <cstring>

#include "../../include/http/http.h"
#include "../../include/helpers/utils.h"

void http::parseRequest(const std::string &request, std::string &parsedMethod, std::string &parsedPath) {
    char method[4];
    int i = 0;
    while ((request[i] != ' ') && (i < request.length()-1) && (request[i] != '\r') && (request[i] != '\n') && (i <= 3)) {
        method[i] = request[i];
        i++;
    }
    method[i] = '\0';
    if ((request[i] == '\0') || (i > 3)) {
        return;
    }
    if (strcmp(method, METHOD_GET) == 0) {
        parsedMethod = METHOD_GET;
    } else if (strcmp(method, METHOD_HEAD) == 0) {
        parsedMethod = METHOD_HEAD;
    } else {
        return;
    }

    i++;
    int copyStart = i;
    while ((request[i] != ' ') && (i < request.length()-1) && (request[i] != '\r') && (request[i] != '\n')) {
        i++;
    }
    int copyEnd = i;
    parsedPath = request.substr(copyStart, copyEnd - copyStart);

    /*long firstPos = parsedPath.find_first_of('%');
    if (firstPos != std::string::npos) {
        std::string filteredPath = parsedPath.substr(0, firstPos);
        bool isPercent = false;
        for (int i = firstPos; i < parsedPath.length(); i++) {
            if (parsedPath[i] == '%') {
                isPercent = true;
            } else if (isPercent) {
                char hexStr[5];
                hexStr[0] = parsedPath[i];
                i++;
                hexStr[1] = parsedPath[i];
                i = i + 2;
                hexStr[2] = parsedPath[i];
                i++;
                hexStr[3] = parsedPath[i];
                hexStr[4] = '\0';
                long hex = strtoul(hexStr, (char **)&hexStr, 16);
                char32_t hexChar = (char32_t)hex;
                filteredPath.append("т");
                isPercent = false;
            }
        }
        parsedPath = filteredPath;
    } */
}

std::string http::makeResponseHead(const std::string &status, const std::string &date,
                                   const std::string &contentType, ssize_t contentLength,
                             const std::string &connection) {
    return (status + "Date: " + date + "\r\nServer: tp-server\r\nContent-Type: " + contentType + "\r\nContent-Length: " +
            std::to_string(contentLength) + "\r\nConnection: " + connection + "\r\n\r\n");
}

std::string http::makeResponse(const std::string &status, const std::string &date,
                               const std::string &contentType, ssize_t contentLength,
                          const std::string &connection, const std::string &body) {
    return (status + "Date: " + date + "\r\nServer: tp-server\r\nContent-Type: " + contentType + "\r\nContent-Length: " +
            std::to_string(contentLength) + "\r\nConnection: " + connection + "\r\n\r\n" + body);
}