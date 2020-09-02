#ifndef SERVER_H
#define SERVER_H

#include <QThread>
#include "definitions.h"
#include "Socket.h"

class gameserver : public QThread {
    //Q_OBJECT
public:
    gameserver();
private:
    void run();
    SocketServer* socksvr;
    Socket* players[5];
};

#endif // SERVER_H
