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
private slots:
    void deal_msg();
    void showwaiting(int);
    void draw_my_cards(set<card>);
    void draw_lord_cards();
    void draw_left_cards(int);
    void draw_right_cards(int);
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
};

#endif // GAMEWINDOW_H
