//
// Created by sibirsky on 27.06.17.
//

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "../../include/server/ClientHandleTask.h"
#include "../../include/http/http.h"
#include "../../include/server/Server.h"

#define CHUNK 256

ClientHandleTask::ClientHandleTask(ClientHandler **client) {
    this->client = *client;
}

ClientHandleTask::~ClientHandleTask() {
    if (client != NULL) {
        delete client;
    }
}

void ClientHandleTask::execute() {
    std::string request = client->receiveRaw();
    if (request.length() == 0) {
        return;
    }
    std::string method;
    std::string path;
    http::parseRequest(request, method, path);
    if ((method.length() == 0) || (path.length() == 0)) {
        client->sendRaw(HTTP400RAW);
        return;
    }
    if ((method != METHOD_GET) && (method != METHOD_HEAD)) {
        client->sendRaw(HTTP405RAW);
        return;
    }

    if (path[path.length()-1] == '/') {
        path.append("index.html");
    }
    std::string fullPath = Server::getRootDir();
    fullPath.append(path);
    int filed = open(fullPath.c_str(), O_RDONLY);
    if (filed <= -1) {
        client->sendRaw(HTTP404RAW);
        return;
    }
    struct stat statBuf;
    if (fstat(filed, &statBuf) != 0) {
        client->sendRaw(HTTP500RAW);
        return;
    }
    if (S_ISDIR(statBuf.st_mode)) {
        client->sendRaw(HTTP404RAW);
        return;
    }

    ssize_t fileSize = statBuf.st_size;
    //TODO: rewrite http and utils classes
    std::string mimeType = "stub"; //util.getMimeType(fullPath);
    std::string date = "stub"; //util.getDateTime();
    client->sendRaw(http::makeResponseHead(STATUS_OK, date, mimeType, fileSize, "Closed").c_str());
    if (method != METHOD_HEAD) {
        char fileBuffer[CHUNK];
        long readBytes, sentBytes;
        while ((readBytes = read(filed, fileBuffer, CHUNK)) > 0) {
            sentBytes = client->sendRaw(fileBuffer, readBytes);
            if (sentBytes < readBytes) {
                break;
            }
        }
    }

    close(filed);
}