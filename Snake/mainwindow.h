#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>
#include <QLabel>
#include <QFileDialog>
#include <cstring>
#include <string>
#include <QPushButton>
#include <QAction>
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

const int SCREEN_SIZE = 800;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

signals:
    void gameover();

private slots:
    void nxt_round();
    void dealgameover();

    void on_b_stt_clicked();

    void on_b_pause_clicked();

    void on_b_quit_clicked();

    void on_actionStart_triggered();

    void on_actionPause_triggered();

    void on_actionQuit_triggered();

    void on_b_save_clicked();

    void on_b_load_clicked();

    void on_actionSave_triggered();

    void on_actionLoad_triggered();

private:
    QTimer* roundtimer;
    QTimer* rendtimer;
    Snakegame game;
    Ui::MainWindow *ui;
    QPixmap screen;
    void Terrain(int, int, int);
    int score;
    void paintEvent(QPaintEvent*);
    void rend();
    void setbools(bool r, bool p, bool q, bool s, bool l);
};
#endif // MAINWINDOW_H
