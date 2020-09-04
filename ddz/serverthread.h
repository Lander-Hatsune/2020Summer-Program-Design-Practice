#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QtNetwork>
#include <QThread>
#include "definitions.h"
#include "ddzgame.h"

class serverthread: public QThread
{
    Q_OBJECT
public:
    serverthread();
signals:
private slots:
    void accept();
private:
    QTcpServer* server;
    QTcpSocket* socks[5];
    int cnt;
    void run();
    ddzgame* game;

    set<card> last_estab;
    int last_estab_player;
};

#endif // SERVERTHREAD_H
