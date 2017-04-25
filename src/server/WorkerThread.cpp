#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include "../../include/server/WorkerThread.h"
#include "../../include/thread/AbstractThreadHandler.h"
#include "../../include/http/http.h"

#define CHUNK 256

WorkerThread::WorkerThread(const char *rootDir) {
    strcpy(this->rootDir, rootDir);
    workState = {false, PTHREAD_MUTEX_INITIALIZER};
}

WorkerThread::~WorkerThread() {
    if (client != NULL) {
        delete client;
    }
}

char* WorkerThread::getRootDir() {
    char* res = new char;
    strcpy(res, rootDir);
    return res;
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

bool WorkerThread::isAvalible() {
    pthread_mutex_lock(&state.mutex);
    bool avalible = state.state == ThreadState::FREE;
    pthread_mutex_unlock(&state.mutex);
    return avalible;
}

void WorkerThread::threadWork() {
    while (true) {
        pthread_mutex_lock(&workState.mutex);
        while (!workState.hasWork) {
            pthread_cond_wait(&workState.cond, &workState.mutex);
        }

        char* request;
        long received;
        client->receiveRaw(&received, &request);
        if (request == NULL) {
            return;
        }
        char* method;
        char* path;
        http::parseRequest(request, &method, &path);
        if ((method == NULL) || (path == NULL)) {
            client->sendRaw(HTTP500RAW);
            delete request, method, path;
            return;
        }

        char* fullPath = getRootDir();
        strcat(fullPath, path);
        int filed = open(fullPath, O_RDONLY);
        if (filed <= -1) {
            client->sendRaw(HTTP404RAW);
            delete request, method, path, fullPath;
            return;
        }
        struct stat statBuf;
        if (fstat(filed, &statBuf) != 0) {
            client->sendRaw(HTTP500RAW);
            delete request, method, path, fullPath;
            return;
        }
        if (S_ISDIR(statBuf.st_mode)) {
            client->sendRaw(HTTP404RAW);
            delete request, method, path, fullPath;
            return;
        }
        ssize_t fileSize = statBuf.st_size;
        client->sendRaw(http::makeResponseHead(STATUS_OK, "text/html", fileSize, "Closed"));

        char fileBuffer[CHUNK];
        long readBytes, sentBytes;
        while ((readBytes =read(filed, fileBuffer, CHUNK)) > 0) {
            sentBytes = client->sendRaw(fileBuffer);
            if (sentBytes < readBytes) {
                break;
            }
        }
        close(filed);
        delete request, method, path, fullPath;
        delete client;

        pthread_mutex_unlock(&workState.mutex);
        pthread_mutex_lock(&state.mutex);
        state.state = ThreadState::FREE;
        pthread_mutex_unlock(&state.mutex);
    }
}