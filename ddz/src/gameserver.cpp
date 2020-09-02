#include "gameserver.h"

gameserver::gameserver()
{
    socksvr = new SocketServer(PORT, 5);
    printf("server: init\n");
}

void gameserver::run()
{
    printf("server: run\n");
    for (int i = 1; i <= 3; i++) {
        players[i] = socksvr->Accept();
        if (players[i] == nullptr) {
            players[i]->SendLine(to_string(i));
            printf("server: get one connection\n");
        }
        else {
            i -= 1;
            printf("server: connected null\n");
        }
    }
    printf("server: all connected\n");
    while (true);
}
