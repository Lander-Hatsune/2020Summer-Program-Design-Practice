#include "initwindow.h"
#include "ui_initwindow.h"

InitWindow::InitWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InitWindow)
{
    ui->setupUi(this);
}

InitWindow::~InitWindow()
{
    delete ui;
}


void InitWindow::on_b_create_clicked()
{
    serverthread* svrthr = new serverthread();
    svrthr->start();

    on_b_join_clicked();
}

void InitWindow::on_b_join_clicked()
{
    QString ip = ui->le_ip->text();
    if (ip.isEmpty()) {
        ip = QString::fromStdString("127.0.0.1");
    }
    GameWindow* gamewindow = new GameWindow(nullptr, ip, PORT);
    this->hide();
    gamewindow->show();
}
