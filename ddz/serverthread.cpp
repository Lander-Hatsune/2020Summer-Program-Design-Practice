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

    // judge the lord
    int cur_lord_to_judge = rand() % 3;
    int lord = cur_lord_to_judge;
    string cur_state = "000";
    for (int i = 0; i < 3; i++) {
        cur_state[cur_lord_to_judge] = '1';
        send(socks[cur_lord_to_judge], GET_DZ + cur_state);
        int ans = recv(socks[cur_lord_to_judge])[0] - '0';
        if (ans) {
            cur_state[cur_lord_to_judge] = '2';
            lord = cur_lord_to_judge;
        }
        cur_lord_to_judge += 1;
        cur_lord_to_judge %= 3;
    }
    while (true);
}
