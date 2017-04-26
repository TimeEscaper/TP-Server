#ifndef TP_SERVER_HTTP_H
#define TP_SERVER_HTTP_H

#include <string>

#define METHOD_GET "GET"
#define METHOD_HEAD "HEAD"

#define STATUS_OK "HTTP/1.1 200 OK\r\n"
#define STATUS_NOT_FOUND "HTTP/1.1 404 Not Found\r\n"
#define STATUS_FORBIDDEN "HTTP/1.1 403 Forbidden\r\n"

#define HTTP404RAW "HTTP/1.1 404 Not Found\r\nContent-Length: 9\r\nContent-Type: text/plain\r\nConnection: Closed\r\n\r\nNot Found"
#define HTTP200RAW "HTTP/1.1 200 OK\r\nContent-Length: 13\r\nContent-Type: text/plain\r\nConnection: Closed\r\n\r\nHello, World!"
#define HTTP500RAW "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 22\r\nContent-Type: text/plain\r\nConnection: Closed\r\n\r\nError parsing request!"


namespace http {
    std::string makeResponseHead(const std::string &status, const std::string &date,
                                 const std::string &contentType, ssize_t contentLength,
                                 const std::string &connection);
    std::string makeResponse(const std::string &status, const std::string &date,
                             const std::string &contentType, ssize_t contentLength,
                       const std::string &connection, const std::string &body);
    void parseRequest(const std::string &request, std::string &parsedMethod, std::string &parsedPath);
}

#endif //TP_SERVER_HTTP_H
