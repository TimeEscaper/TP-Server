#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <libgen.h>
#include <climits>
#include "../../include/server/ClientHandleTask.h"
#include "../../include/http/http.h"
#include "../../include/server/Server.h"

#define CHUNK 256

ClientHandleTask::ClientHandleTask(ClientHandler **client, std::string& rootDir) {
    this->client = *client;
    this->rootDir = rootDir;
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
    Http::parseRequest(request, method, path);
    if ((method.length() == 0) || (path.length() == 0)) {
        client->sendRaw(HTTP400RAW);
        return;
    }
    if ((method != METHOD_GET) && (method != METHOD_HEAD)) {
        client->sendRaw(HTTP405RAW);
        return;
    }



    bool needIndex = false;
    if (path[path.length()-1] == '/') {
        path.append("index.html");
        needIndex = true;
    }
    std::string fullPath = rootDir;
    fullPath.append(path);
   /* if (path.find('%') != std::string::npos) {
        fullPath.append(Http::urlDecode(path));
    } else {
        fullPath.append(path);
    }*/

    char absPath[PATH_MAX];
    if (realpath(fullPath.c_str(), absPath) == NULL) {
        if (needIndex) {
            client->sendRaw(HTTP403RAW);
            return;
        }
        client->sendRaw(HTTP404RAW);
        return;
    }
    std::string filteringString = absPath;
    if (filteringString.find(rootDir) == std::string::npos) {
        client->sendRaw(HTTP404RAW);
        return;
    }

    int filed = open(absPath, O_RDONLY);
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
    std::string mimeType = Http::getMimeType(fullPath);
    std::string date = Http::getDateTime();
    client->sendRaw(Http::makeResponseHead(STATUS_OK, date, mimeType, fileSize, "Closed").c_str());
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