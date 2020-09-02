#include "initwindow.h"
#include "ui_initwindow.h"

InitWindow::InitWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InitWindow)
{
    ui->setupUi(this);
    printf("initwindow\n");
}

InitWindow::~InitWindow()
{
    delete ui;
}


void InitWindow::on_b_join_clicked()
{
    QDialog* joindlg = new JoinDlg(this);
    joindlg->show();
    this->hide();
}

void InitWindow::on_b_create_clicked()
{
    QThread* svrthr = new gameserver();
    svrthr->start();
    SocketClient* clt = new SocketClient("127.0.0.1", PORT);
    printf("client: connected\n");
    QWidget* gamewindow = new GameWindow(this, clt);
    gamewindow->show();
    this->hide();
}
