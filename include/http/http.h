#ifndef TP_SERVER_HTTP_H
#define TP_SERVER_HTTP_H

#define HTTP404RAW "HTTP/1.1 404 Not Found\r\nContent-Length: 9\r\nContent-Type: text/plain\r\nConnection: Closed\r\n\r\nNot Found"
#define HTTP200RAW "HTTP/1.1 200 OK\r\nContent-Length: 13\r\nContent-Type: text/plain\r\nConnection: Closed\r\n\r\nHello, World!"

namespace http {

}

#endif //TP_SERVER_HTTP_H
