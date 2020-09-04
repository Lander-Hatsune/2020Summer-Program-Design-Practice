#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QtNetwork>
#include "definitions.h"
#include "tcpmsg.h"
#include "ddzgame.h"
#include <string>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
using namespace std;

namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr,
                        QString ip = "",
                        int port = 40011);
    ~GameWindow();
signals:
    void waiting(int);
    void got_first_cards(set<card>);
    void got_lord_cards();
    void left_cards(int);
    void right_cards(int);
    void wait_dz(string);
    void get_dz(string);
    void is_valid();
    void restart(bool);

private slots:
    void deal_msg();
    void showwaiting(int);
    void draw_my_cards(set<card>);
    void draw_lord_cards();
    void draw_left_cards(int);
    void draw_right_cards(int);
    void deal_wait_dz(string);
    void deal_get_dz(string);
    void on_b_judgelord_clicked();
    void on_b_notjudgelord_clicked();
    void on_b_establish_clicked();
    void on_b_pass_clicked();
    void judge_valid();

private:
    QPushButton* b_cards[25];
    card btn_to_card[25];
    QLabel* lbl_lord_card[3];
    QLabel* lbl_left_cards[25];
    QLabel* lbl_right_cards[25];
    set<card> my_cards;
    set<card> lord_cards;
    QTcpSocket* sock;
    Ui::GameWindow *ui;
    int number;
    QLabel* lbl_state[3];
    int judgelord;
    bool is_rob;

    QLabel* lbl_last_estab[25];
    set<card> ready_estab;
    set<card> last_estab;
    int last_player;
    int cur_player;

    void show_last_cards();

    QLabel* indicator[4];
};

#endif // GAMEWINDOW_H
