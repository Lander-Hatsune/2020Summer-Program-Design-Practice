#include "ddzgame.h"
#include <iostream>

ddzgame::ddzgame() {
    srand(time(NULL));
    card allcards[60];
    int cnt = 0;
    for (int c = 1; c <= 4; c++) {
        for (int v = 3; v <= 15; v++) {
            allcards[cnt++] = (card){c, v};
        }
    }
    allcards[52] = (card){0, 16};// black joker
    allcards[53] = (card){0, 17};// red joker
    printf("game: all cards generated\n");

    random_shuffle(allcards, allcards + 54);
    printf("game: cards shuffled\n");

    for (int i = 51; i < 54; i++) {
        lord_s[i - 51] = allcards[i];
    }
    for (int i = 0; i < 51; i++) {
        player_cards[i / 17].insert(allcards[i]);
    }
}

bool ddzgame::greater(set<card> old, set<card> cur) {
    
}

bool ddzgame::establish(int num, set<card> cards) {
    while (cards.size()) {
        card cur = *(cards.begin());
        printf("established: %d, %d\n", cur.color, cur.val);
        player_cards[num].erase(cur);
        cards.erase(cur);
    }
}

string ddzgame::get_player_cards(int num) {
    string ret = "";
    for (set<card>::iterator it = player_cards[num].begin();
         it != player_cards[num].end(); ++it) {
        ret += to_string((*it).color) + (char)('a' + (*it).val - 1);
    }
    printf("game: player %d's cards: ", num);
    cout << ret << endl;
    return ret;
}

string ddzgame::get_lord_cards()
{
    string ret = "";
    for (int i = 0; i < 3; i++) {
        ret += to_string(lord_s[i].color) + (char)('a' + lord_s[i].val - 1);
    }
    return ret;
}

void ddzgame::setlord(int lord) {
    this->lord = lord;
}
