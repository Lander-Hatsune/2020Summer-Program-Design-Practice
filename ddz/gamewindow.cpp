#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <iostream>

const int left_bias = 190, up_bias = 540;
const int card_width = 150, card_height = 220;

string get_cardname(int color, int val) {
    string name;
    if (color == 0) {
        name = (char)('a' + val - 1);
    }
    else {
        name = to_string(color) + (char)('a' + val - 1);
    }
    //printf("card: %d, %d; name: ", color, val);
    //cout << name << endl;
    return name;
}

string get_cardname(card c) {
    return get_cardname(c.color, c.val);
}

GameWindow::GameWindow(QWidget *parent,
                       QString ip,
                       int port) :
    QWidget(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    this->sock = new QTcpSocket();
    printf("client: connected\n");
    connect(sock, SIGNAL(readyRead()), this, SLOT(deal_msg()));
    connect(this, SIGNAL(waiting(int)), this, SLOT(showwaiting(int)));
    connect(this, SIGNAL(got_first_cards(set<card>)), this, SLOT(draw_my_cards(set<card>)));
    connect(this, SIGNAL(got_lord_cards()), this, SLOT(draw_lord_cards()));
    connect(this, SIGNAL(left_cards(int)), this, SLOT(draw_left_cards(int)));
    connect(this, SIGNAL(right_cards(int)), this, SLOT(draw_right_cards(int)));
    sock->connectToHost(ip, port);
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::deal_msg()
{
    string msg = recv(sock);
    if (msg.empty() || msg.size() < 4) {
        printf("msg invalid\n");
        return;
    }
    string head = msg.substr(0, 4);
    msg = msg.substr(4);
    if (head == WAIT) {
        int cnt = msg[0] - '0';
        emit waiting(cnt);
    }
    else if (head == NO) {
        number = msg[0] - '0';
    }
    else if (head == STT) {
        for (int i = 0; i < 34; i += 2) {
            card cur;
            cur.color = msg[i] - '0';
            cur.val = msg[i + 1] - 'a' + 1;
            my_cards.insert((cur));
            //printf("card %d, color: %d, val: %d\n", i / 2, cur.color, cur.val);
        }
        for (int i = 34; i < msg.length(); i += 2) {
            card cur;
            cur.color = msg[i] - '0';
            cur.val = msg[i + 1] - 'a' + 1;
            lord_cards.insert(cur);
        }
        emit got_first_cards(my_cards);
        emit got_lord_cards();
        //emit left_cards(17);
        //emit right_cards(17);
    }
    else if (head == GET_DZ) {
        //emit get_dz(msg);
    }
    if (sock->bytesAvailable() > 0) {
        printf("left %d bytes\n", sock->bytesAvailable());
        deal_msg();
    }
}

void GameWindow::showwaiting(int cnt)
{
    printf("show waiting\n");
    ui->lbl_waiting->setText("<html><head/><body><p><span style=\" font-size:36pt; color:#fcaf3e;\">等待玩家就绪:" +
                             QString::fromStdString(to_string(cnt)) + "</span></p></body></html>");
}

void GameWindow::draw_my_cards(set<card> cards)
{
    ui->lbl_waiting->setText("");
    int i = 0;
    for (set<card>::iterator it = cards.begin();
         it != cards.end(); it++, i++) {
        b_cards[i] = new QPushButton(this);
        b_cards[i]->setGeometry(left_bias + i * 50, up_bias, card_width, card_height);
        QPixmap pxmap(QPixmap(tr((":/new/card/resources/" + get_cardname(*it) + ".png").c_str())));
        b_cards[i]->setIcon(pxmap);
        b_cards[i]->setIconSize(b_cards[i]->size());
        b_cards[i]->show();
        btn_to_card[i] = *it;
        connect(b_cards[i], SIGNAL(toggled(bool)), this, SLOT(card_toggled(bool)));
    }
}

void GameWindow::draw_lord_cards()
{
    for (int i = 0; i < 3; i++) {
        lbl_lord_card[i] = new QLabel(this);
        lbl_lord_card[i]->setGeometry(420 + i * card_width / 1.5, 40, card_width / 1.5, card_height / 1.5);
        QPixmap bgpx(tr(":/new/card/resources/back.png"));
        lbl_lord_card[i]->setPixmap(bgpx);
        lbl_lord_card[i]->show();
    }
}

void GameWindow::draw_left_cards(int num)
{
    for (int i = 0; i < 25; i++) {
        if (lbl_left_cards[i]) {
            //lbl_left_cards[i]->hide();
            delete lbl_left_cards[i];
        }
    }
    for (int i = 0; i < num; i++) {
        lbl_left_cards[i] = new QLabel();
        lbl_left_cards[i]->setGeometry(10, 10 + 20 * i, card_width / 2, card_height / 2);
        QPixmap bgpx(tr(":/new/card/resources/back.png"));
        lbl_left_cards[i]->setPixmap(bgpx);
        lbl_left_cards[i]->show();
    }
}

void GameWindow::draw_right_cards(int num)
{
    for (int i = 0; i < 25; i++) {
        if (lbl_right_cards[i]) {
            //lbl_right_cards[i]->hide();
            delete lbl_right_cards[i];
        }
    }
    for (int i = 0; i < num; i++) {
        lbl_right_cards[i] = new QLabel();
        lbl_right_cards[i]->setGeometry(1140, 10 + 20 * i, card_width / 2, card_height / 2);
        QPixmap bgpx(tr(":/new/card/resources/back.png"));
        lbl_right_cards[i]->setPixmap(bgpx);
        lbl_right_cards[i]->show();
    }
}
