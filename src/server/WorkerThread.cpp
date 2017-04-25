#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include "../../include/server/WorkerThread.h"
#include "../../include/thread/AbstractThreadHandler.h"
#include "../../include/http/http.h"
#include "../../include/helpers/utils.h"

#define CHUNK 256

WorkerThread::WorkerThread(const std::string &rootDir) {
    this->rootDir = rootDir;
    workState = {false, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
}

WorkerThread::~WorkerThread() {
    if (client != NULL) {
        delete client;
    }
}

std::string WorkerThread::getRootDir() {
    return rootDir;
}

void WorkerThread::handleClient(ClientHandler **newClient) {
    pthread_mutex_lock(&state.mutex);
    if (state.state == ThreadState::FREE) {
        client = *newClient;
        state.state = ThreadState::BUSY;
        pthread_mutex_lock(&workState.mutex);
        workState.hasWork = true;
        pthread_cond_signal(&workState.cond);
        pthread_mutex_unlock(&workState.mutex);
    }
    pthread_mutex_unlock(&state.mutex);
}

bool WorkerThread::isAvailable() {
    pthread_mutex_lock(&state.mutex);
    bool avalible = state.state == ThreadState::FREE;
    pthread_mutex_unlock(&state.mutex);
    return avalible;
}

void WorkerThread::processClient() {
    std::string request = client->receiveRaw();
    if (request.length() == 0) {
        return;
    }
    std::string method;
    std::string path;
    http::parseRequest(request, method, path);
    if ((method.length() == 0) || (path.length() == 0)) {
        client->sendRaw(HTTP500RAW);
        return;
    }

    std::string fullPath = getRootDir();
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
    std::string mimeType = utils::getMimeType(fullPath);
    client->sendRaw(http::makeResponseHead(STATUS_OK, mimeType, fileSize, "Closed").c_str());

    char fileBuffer[CHUNK];
    long readBytes, sentBytes;
    while ((readBytes = read(filed, fileBuffer, CHUNK)) > 0) {
        sentBytes = client->sendRaw(fileBuffer, readBytes);
        if (sentBytes < readBytes) {
            break;
        }
    }

    close(filed);
}

void WorkerThread::threadWork() {
    while (true) {
        pthread_mutex_lock(&workState.mutex);
        while (!workState.hasWork) {
            pthread_cond_wait(&workState.cond, &workState.mutex);
        }
        processClient();
        delete client;
        client = NULL;
        pthread_mutex_lock(&state.mutex);
        state.state = ThreadState::FREE;
        workState.hasWork = false;
        pthread_mutex_unlock(&state.mutex);
        pthread_mutex_unlock(&workState.mutex);
    }
}