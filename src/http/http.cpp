#include <cstdlib>
#include <cstring>

#include "../../include/http/http.h"
#include "../../include/helpers/utils.h"

void http::parseRequest(const char *request, char **parsedMethod, char **parsedPath) {
    char method[4];
    int i = 0;
    while ((request[i] != ' ') && (request[i] != '\0') && (request[i] != '\r') && (request[i] != '\n') && (i <= 3)) {
        method[i] = request[i];
        i++;
    }
    method[i] = '\0';
    if ((request[i] == '\0') || (i > 3)) {
        *parsedMethod = NULL;
        *parsedPath = NULL;
        return;
    }
    if (strcmp(method, METHOD_GET) == 0) {
        *parsedMethod = new char[4];
        strcpy(*parsedMethod, METHOD_GET);
    } else if (strcmp(method, METHOD_HEAD) == 0) {
        *parsedMethod = new char[5];
        strcpy(*parsedMethod, METHOD_HEAD);
    } else {
        *parsedMethod = NULL;
        *parsedPath = NULL;
        return;
    }

    i++;
    int copyStart = i;
    while ((request[i] != ' ') && (request[i] != '\0') && (request[i] != '\r') && (request[i] != '\n')) {
        i++;
    }
    if ((request[i] == '\0')) {
        *parsedPath = NULL;
        return;
    }
    int copyEnd = i;
    *parsedPath = new char[copyEnd - copyStart];
    strncpy(*parsedPath, &request[copyStart], (copyEnd - copyStart)*sizeof(char));
    (*parsedPath)[copyEnd - copyStart] = '\0';
}

char* http::makeResponseHead(const char *status, const char *contentType, ssize_t contentLength,
                             const char *connection) {
    char* response = new char;
    strcat(response, status);
    strcat(response, "Server: tp-server\r\n");
    strcat(response, "Content-Type: ");
    strcat(response, contentType);
    strcat(response, "\r\nContent-Length: ");
    char contentLengthStr[15];
    sprintf(contentLengthStr, "%ld", contentLength);
    strcat(response, contentLengthStr);
    strcat(response, "\r\nConnection: ");
    strcat(response, connection);
    strcat(response, "\r\n\r\n");
}

char* http::makeResponse(const char *status, const char *contentType, ssize_t contentLength, const char *connection,
                         const char *body) {
    char* response = (char *) "";
    strcat(response, status);
    strcat(response, "Server: tp-server\r\n");
    strcat(response, "Content-Type: ");
    strcat(response, contentType);
    strcat(response, "\r\nContent-Length: ");
    char contentLengthStr[15] = "";
    sprintf(contentLengthStr, "%ld", contentLength);
    strcat(response, contentLengthStr);
    strcat(response, "\r\nConnection: ");
    strcat(response, connection);
    strcat(response, "\r\n\r\n");
    strcat(response, body);
}