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
}

std::string http::makeResponseHead(const std::string &status, const std::string &contentType, ssize_t contentLength,
                             const std::string &connection) {
    return (status + "Server: tp-server\r\nContent-Type: " + contentType + "\r\nContent-Length: " +
            std::to_string(contentLength) + "\r\nConnection: " + connection + "\r\n\r\n");
}

std::string http::makeResponse(const std::string &status, const std::string &contentType, ssize_t contentLength,
                          const std::string &connection, const std::string &body) {
    return (status + "Server: tp-server\r\nContent-Type: " + contentType + "\r\nContent-Length: " +
            std::to_string(contentLength) + "\r\nConnection: " + connection + "\r\n\r\n" + body);
}