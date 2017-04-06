#ifndef TP_SERVER_HTTP_H
#define TP_SERVER_HTTP_H

#define METHOD_GET "GET"
#define METHOD_HEAD "HEAD"

#define STATUS_OK "HTTP/1.1 200 OK\r\n"
#define STATUS_NOT_FOUND "HTTP/1.1 404 Not Found\r\n"
#define STATUS_FORBIDDEN "HTTP/1.1 403 Forbidden\r\n"

#define HTTP404RAW "HTTP/1.1 404 Not Found\r\nContent-Length: 9\r\nContent-Type: text/plain\r\nConnection: Closed\r\n\r\nNot Found"
#define HTTP200RAW "HTTP/1.1 200 OK\r\nContent-Length: 13\r\nContent-Type: text/plain\r\nConnection: Closed\r\n\r\nHello, World!"
#define HTTP500RAW "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 22\r\nContent-Type: text/plain\r\nConnection: Closed\r\n\r\nError parsing request!"


namespace http {
    char* makeResponseHead(const char* status, const char* contentType, ssize_t contentLength,
                           const char* connection);
    char* makeResponse(const char* status, const char* contentType, ssize_t contentLength,
                       const char* connection, const char* body);
    void parseRequest(const char* request, char** parsedMethod, char** parsedPath);
}

#endif //TP_SERVER_HTTP_H
