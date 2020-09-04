#include "initwindow.h"
#include "ui_initwindow.h"

InitWindow::InitWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InitWindow)
{
    ui->setupUi(this);
    svrthr = 0;
}

InitWindow::~InitWindow()
{
    delete ui;
}


void InitWindow::on_b_create_clicked()
{
    svrthr = new serverthread();
    svrthr->start();

    on_b_join_clicked();
}

void InitWindow::on_b_join_clicked()
{
    if (!ip.isEmpty()) {

    }
    else {
        ip = ui->le_ip->text();
        if (ip.isEmpty()) {
            ip = QString::fromStdString("127.0.0.1");
        }
    }
    GameWindow* gamewindow = new GameWindow(nullptr, ip, PORT);
    connect(gamewindow, SIGNAL(restart(bool)), this, SLOT(deal_restart(bool)));
    this->hide();
    gamewindow->show();
}

void InitWindow::deal_restart(bool ishost)
{
    /*
    if (ishost) {
        on_b_create_clicked();
    }
    else {
        on_b_join_clicked();
    }
    */
    this->show();
}
