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
    memset(lbl_lord_card, 0, sizeof(lbl_lord_card));
    memset(lbl_left_cards, 0, sizeof(lbl_left_cards));
    memset(lbl_right_cards, 0, sizeof(lbl_right_cards));
    ui->b_pass->hide();
    ui->b_establish->hide();
    ui->b_judgelord->hide();
    ui->b_notjudgelord->hide();
    judgelord = 0;

    connect(sock, SIGNAL(readyRead()), this, SLOT(deal_msg()));
    connect(this, SIGNAL(waiting(int)), this, SLOT(showwaiting(int)));
    connect(this, SIGNAL(got_first_cards(set<card>)), this, SLOT(draw_my_cards(set<card>)));
    connect(this, SIGNAL(got_lord_cards()), this, SLOT(draw_lord_cards()));
    connect(this, SIGNAL(left_cards(int)), this, SLOT(draw_left_cards(int)));
    connect(this, SIGNAL(right_cards(int)), this, SLOT(draw_right_cards(int)));
    connect(this, SIGNAL(wait_dz(string)), this, SLOT(deal_wait_dz(string)));
    connect(this, SIGNAL(get_dz(string)), this, SLOT(deal_get_dz(string)));
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
        this->setWindowTitle(QString::fromStdString(msg));
        for (int i = 0; i < 3; i++) {
            lbl_state[i] = new QLabel(this);
        }
        lbl_state[number]->setGeometry(580, 460, 100, 50);
        lbl_state[(number + 1) % 3]->setGeometry(1000, 300, 200, 50);
        lbl_state[(number + 2) % 3]->setGeometry(200, 300, 200, 50);
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
        emit left_cards(17);
        emit right_cards(17);
    }
    else if (head == WAIT_DZ) {
        emit wait_dz(msg);
    }
    else if (head == GET_DZ) {
        emit get_dz(msg);
    }
    else if (head == DZ) {
        for (int i = 0; i < 3; i++) {
            lbl_state[i]->setText("");
        }
        int lord = msg[0] - '0';
        QLabel* lbl_dz = new QLabel(this);
        lbl_dz->setText("===地主===");
        if (lord == number) {
            lbl_dz->setGeometry(270, 460, 100, 50);
            for (set<card>::iterator it = lord_cards.begin();
                 it != lord_cards.end(); it++) {
                my_cards.insert(*it);
            }
            draw_my_cards(my_cards);
        }
        else if (lord == (number + 1) % 3) {
            lbl_dz->setGeometry(1000, 60, 100, 50);
            draw_right_cards(20);
        }
        else if (lord == (number + 2) % 3) {
            lbl_dz->setGeometry(180, 60, 100, 50);
            draw_left_cards(20);
        }
        lbl_dz->show();
        int i = 0;
        for (set<card>::iterator it = lord_cards.begin();
             it != lord_cards.end(); it++, i++) {
            QPixmap pxmap(QPixmap(tr((":/new/card/resources/" + get_cardname(*it) + ".png").c_str())));
            lbl_lord_card[i]->setPixmap(pxmap);
        }
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
        connect(b_cards[i], &QPushButton::clicked,
                [=]()
        {
            QRect ori_pos = b_cards[i]->geometry();
            if (ori_pos.y() > 520) {
                b_cards[i]->setGeometry(ori_pos.x(), ori_pos.y() - 40,
                                        ori_pos.width(), ori_pos.height());
                ready_estab.insert(btn_to_card[i]);
            }
            else {
                b_cards[i]->setGeometry(ori_pos.x(), ori_pos.y() + 40,
                                        ori_pos.width(), ori_pos.height());
                ready_estab.erase(btn_to_card[i]);
            }
        });
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
            lbl_left_cards[i]->hide();
        }
    }
    for (int i = 0; i < num; i++) {
        lbl_left_cards[i] = new QLabel(this);
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
            lbl_right_cards[i]->hide();
        }
    }
    for (int i = 0; i < num; i++) {
        lbl_right_cards[i] = new QLabel(this);
        lbl_right_cards[i]->setGeometry(1140, 10 + 20 * i, card_width / 2, card_height / 2);
        QPixmap bgpx(tr(":/new/card/resources/back.png"));
        lbl_right_cards[i]->setPixmap(bgpx);
        lbl_right_cards[i]->show();
    }
}

void GameWindow::deal_wait_dz(string state)
{
    for (int i = 0; i < 3; i++) {
        if (state[i] == '1') {
            lbl_state[i]->setText("叫地主中...");
            lbl_state[i]->show();
        }
        else if (state[i] == '2') {
            lbl_state[i]->setText("叫地主!");
            lbl_state[i]->show();
        }
        else if (state[i] == '3') {
            lbl_state[i]->setText("不叫");
            lbl_state[i]->show();
        }
    }
}

void GameWindow::deal_get_dz(string state)
{
    is_rob = false;
    for (int i = 0; i < 3; i++) {
        if (state[i] == '2') {
            if (is_rob) {
                lbl_state[i]->setText("抢地主!");
            }
            else {
                lbl_state[i]->setText("叫地主!");
            }
            is_rob = true;
        }
        else if (state[i] == '3') {
            lbl_state[i]->setText("不叫");
        }
        lbl_state[i]->show();
    }
    ui->b_judgelord->show();
    ui->b_notjudgelord->show();
}

void GameWindow::on_b_judgelord_clicked()
{
    send(sock, (string)"1");
    if (is_rob) {
        lbl_state[number]->setText("抢地主!");
    }
    else {
        lbl_state[number]->setText("叫地主!");
    }
    lbl_state[number]->show();
    ui->b_judgelord->hide();
    ui->b_notjudgelord->hide();
}

void GameWindow::on_b_notjudgelord_clicked()
{
    send(sock, (string)"0");
    lbl_state[number]->setText("不叫");
    lbl_state[number]->show();
    ui->b_judgelord->hide();
    ui->b_notjudgelord->hide();
}