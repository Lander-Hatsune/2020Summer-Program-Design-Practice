#ifndef TCPMSG_H
#define TCPMSG_H

#include <QtNetwork>
#include <string>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // _WIN32

using namespace std;

inline void sleepcp(int milliseconds)
{
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif // _WIN32
}

inline void send(QTcpSocket* sock, string msg) {
    //sleepcp(100);
    sock->waitForBytesWritten(-1);
    sock->write((msg + "*MK*").c_str());
    sock->flush();
    printf("sent: ");
    cout << msg + "*MK*" << endl;
    //sleepcp(100);
}

inline void send(QTcpSocket* sock, QString msg) {
    string msg_str = msg.toStdString();
    send(sock, msg_str);
}

inline string recv(QTcpSocket* sock) {
    string ret = "";
    while (true) {
        ret += (sock->read(1)).toStdString();
        //printf("partial ret: ");
        //cout << ret << endl;
        if (ret.length() >= 4 && ret.substr(ret.length() - 4) == "*MK*") {
            printf("recv: ");
            cout << ret << endl;
            return ret.substr(0, ret.length() - 4);
        }
    }
}
#endif // TCPMSG_H
