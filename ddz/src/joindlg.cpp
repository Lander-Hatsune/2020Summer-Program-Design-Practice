#include "joindlg.h"
#include "ui_joindlg.h"

JoinDlg::JoinDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JoinDlg)
{
    this->parent = parent;
    ui->setupUi(this);
}

JoinDlg::~JoinDlg()
{
    delete ui;
}

void JoinDlg::on_b_ok_clicked()
{
    QString ip = ui->le_ip->text();
    if (ip.isEmpty()) {
        ip = QString::fromStdString("127.0.0.1");
    }
    ui->lbl_tip->setText("正在连接");
    SocketClient* clt = new SocketClient(ip.toStdString(), PORT);
    GameWindow* gamewindow = new GameWindow(nullptr, clt);
    gamewindow->show();
    this->hide();
}

void JoinDlg::on_b_cancel_clicked()
{
    this->hide();
    parent->show();
}
