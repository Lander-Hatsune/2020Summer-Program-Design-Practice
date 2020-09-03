#include "serverthread.h"
#include "tcpmsg.h"

serverthread::serverthread()
{
    server = new QTcpServer();
    server->listen(QHostAddress::Any, PORT);
    printf("server ready\n");
    connect(server, SIGNAL(newConnection()), this, SLOT(accept()));
}

void serverthread::accept()
{
    printf("server: new client\n");
    QTcpSocket* sock = server->nextPendingConnection();
    socks[cnt++] = sock;
    send(sock, NO + to_string(cnt - 1));
    if (cnt < 3) {
        for (int i = 0; i < cnt; i++) {
            send(socks[i], WAIT + to_string(cnt));
        }
    }
}

void serverthread::run()
{
    cnt = 0;
    while (cnt < 3) {
        sleepcp(200);
    }
    game = new ddzgame();
    for (int i = 0; i < 3; i++) {
        string player_cards = game->get_player_cards(i);
        string lord_cards = game->get_lord_cards();
        printf("send to %d\n", i);
        send(socks[i], STT + player_cards + lord_cards);
        //send(socks[i], (string)"asd");
    }
    sleepcp(1000);
    // judge the lord

    srand(time(NULL));
    int cur_lord_to_judge = 1;
    int lord = cur_lord_to_judge;
    string cur_state = "000";
    for (int i = 0; i < 3; i++) {
        cur_state[cur_lord_to_judge] = '1';
        printf("server: cur lord to judge: %d\n", cur_lord_to_judge);
        for (int j = 0; j < 3; j++) {
            if (j == cur_lord_to_judge)
                continue;
            send(socks[j], WAIT_DZ + cur_state);
        }
        send(socks[cur_lord_to_judge], GET_DZ + cur_state);
        string ans_str;
        socks[cur_lord_to_judge]->waitForReadyRead();
        ans_str = recv(socks[cur_lord_to_judge]);

        int ans = ans_str[0] - '0';
        printf("got ans: %d\n", ans);
        //int ans = 0;
        if (ans) {
            cur_state[cur_lord_to_judge] = '2';
            lord = cur_lord_to_judge;
        }
        else {
            cur_state[cur_lord_to_judge] = '3';
        }
        cur_lord_to_judge += 1;
        cur_lord_to_judge %= 3;
    }
    for (int i = 0; i < 3; i++) {
        send(socks[i], DZ + to_string(lord));
    }

    while (true);
}