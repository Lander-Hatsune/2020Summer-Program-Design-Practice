#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QtNetwork>
#include "Socket.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr, SocketClient* clt = nullptr);
    ~GameWindow();

private:
    Ui::GameWindow *ui;
    SocketClient* clt;
    int number;
};

#endif // GAMEWINDOW_H
