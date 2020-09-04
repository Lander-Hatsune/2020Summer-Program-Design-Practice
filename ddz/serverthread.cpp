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
        socks[cur_lord_to_judge]->waitForReadyRead(-1);
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

    // start
    last_estab.clear();
    last_estab_player = lord;
    int cur_player = lord;
    int winner = 3;
    while (true) {
        string sizes = "";
        for (int i = 0; i < 3; i++) {
            sizes += to_string(game->player_cards[i].size());
            if (i != 2) sizes += ":";
        }
        string last_estab_str = ddzgame::get_str_from_cards(last_estab);
        for (int i = 0; i < 3; i++) {
            send(socks[i], NXT_RND + sizes + ":" +
                 to_string(last_estab_player) + ":" +
                 to_string(cur_player) + ":" + last_estab_str);

            cout << "server send to " << i << " " << NXT_RND + sizes + ":" +
                    to_string(last_estab_player) + ":" +
                    to_string(cur_player) + ":" + last_estab_str << endl;
        }
        send(socks[cur_player], UR_TURN);
        cout << "server send to " << cur_player << " " << UR_TURN << endl;
        socks[cur_player]->waitForReadyRead(-1);
        string ans = recv(socks[cur_player]);
        cout << "server recv ans: " << ans << endl;
        if (ans[0] == '0') {
            ;
        }
        else {
            last_estab_player = cur_player;
            last_estab = ddzgame::get_cards_from_str(ans.substr(0, ans.find(":")));
            game->establish(last_estab_player, last_estab);
            ans = ans.substr(ans.find(':') + 1);
            if (ans[0] == '1') {
                winner = cur_player;
                break;
            }
        }
        (cur_player += 1) %= 3;
    }
    if (lord == winner) {
        for (int i = 0; i < 3; i++) {
            if (i != winner)
                send(socks[i], FAIL);
            else send(socks[i], SUCC);
        }
    }
    else {
        for (int i = 0; i < 3; i++) {
            if (i != lord)
                send(socks[i], SUCC);
            else send(socks[i], FAIL);
        }
    }
}
