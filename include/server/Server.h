#ifndef TP_SERVER_SERVER_H
#define TP_SERVER_SERVER_H

class Server {
private:
    int port;

public:
    Server(int port);
    ~Server();
    int getPort();
    void start();
    void stop();
    void restart();
};


#endif //TP_SERVER_SERVER_H
