#ifndef INITWINDOW_H
#define INITWINDOW_H

#include <QMainWindow>
#include "serverthread.h"
#include "gamewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class InitWindow; }
QT_END_NAMESPACE

class InitWindow : public QMainWindow
{
    Q_OBJECT

public:
    InitWindow(QWidget *parent = nullptr);
    ~InitWindow();

private slots:
    void on_b_create_clicked();

    void on_b_join_clicked();

    void deal_restart(bool);

private:
    Ui::InitWindow *ui;
    serverthread*  svrthr;
    QString ip;
};
#endif // INITWINDOW_H
