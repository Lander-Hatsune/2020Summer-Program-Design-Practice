#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent, SocketClient* clt) :
    QWidget(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    this->clt = clt;
    number = clt->ReceiveLine()[0] - '0';
}

GameWindow::~GameWindow()
{
    delete ui;
}
