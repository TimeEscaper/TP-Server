#ifndef TP_SERVER_FILEOBSERVER_H
#define TP_SERVER_FILEOBSERVER_H

#define FILE_BUFFER_SIZE 256

class FileObserver {
protected:
    long int getFileSize(int filed);

public:
    virtual ~getFileSize();
};


#endif //TP_SERVER_FILEOBSERVER_H
