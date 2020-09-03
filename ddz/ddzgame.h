#pragma once
#include <string>
#include <set>
#include <algorithm>
#include <vector>
using namespace std;

#define CLUB 1
#define DIAMOND 2
#define HEART 3
#define SPADE 4

typedef struct card {
    int color, val;// 3 -> 13 -> A, 2 -> jokers; 3 -> 17, c -> q
    bool operator < (const struct card& x) const {
        if (val != x.val) 
            return val < x.val;
        else return color < x.color;
    }
}card;

class ddzgame {
public:
    ddzgame();
    set<card> player_cards[3];
    card lord_s[3];
    int lord;
    static bool greater(set<card> old, set<card> cur);
    bool establish(int num, set<card> cards);
    string get_player_cards(int num);
    string get_lord_cards();
    void setlord(int);
};