#ifndef INITWINDOW_H
#define INITWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include "joindlg.h"
#include "gameserver.h"
#include <cstdio>

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
    void on_b_join_clicked();

    void on_b_create_clicked();

private:
    Ui::InitWindow *ui;
};
#endif // INITWINDOW_H
