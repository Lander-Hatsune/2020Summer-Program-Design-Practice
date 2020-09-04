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

int has_geq(multiset<int> cards, int times) {
    for (multiset<int>::iterator it = cards.begin();
         it != cards.end(); ++it) {
        if (cards.count(*it) >= times) {
            return *it;
        }
    }
    return false;
}

int has(multiset<int> cards, int times) {
    for (multiset<int>::iterator it = cards.begin();
         it != cards.end(); ++it) {
        if (cards.count(*it) == times) {
            return *it;
        }
    }
    return false;
}

bool cntinue(multiset<int> cards, int times) {
    int curval = *cards.begin();

    if (times == 1) {
        if (cards.count(15) || cards.size() < 5 || cards.size() > 12)
            return false;
    }
    
    if (times == 2) {
        if (cards.count(13) || cards.count(14) || cards.count(15) ||
            cards.size() < 6 || cards.size() > 20)
            return false;
    }
    
    int continuous = 0;
    while (true) {
        if (cards.count(curval) == times) {
            while (cards.count(curval))
                cards.erase(curval);
            curval += 1;
            continuous += 1;
        }
        else if (cards.empty()) {
            if (times == 1) {
                if (continuous >= 5)
                    return true;
                else return false;
            }
            else if (times == 2) {
                if (continuous >= 3 && continuous <= 10)
                    return true;
                else return false;
            }
        }
        else return false;
    }
}

bool dopio(multiset<int> cards, int how_many) {
    while (how_many--) {
        if (!has(cards, 2)) {
            return false;
        }
        int cur = has(cards, 2);
        cards.erase(cur);
        cards.erase(cur);
    }
    return true;
}

int jet_body(multiset<int> cards) {
    if (!has_geq(cards, 3)) return 0;
    int cur = has_geq(cards, 3);
    int ret = 0;
    while (cards.count(cur) >= 3) {
        for (int i = 1; i <= 3; i++)
            cards.erase(cur);
        cur += 1;
        ret += 1;
    }
    return ret;
}

int jet_val(multiset<int> cards) {
    return has_geq(cards, 3);
}

bool ddzgame::greater(set<card> old_set, set<card> cur_set) {
    multiset<int> old, cur;
    
    for (set<card>::iterator it = old_set.begin();
         it != old_set.end(); ++it) {
        old.insert((*it).val);
    }
    for (set<card>::iterator it = cur_set.begin();
         it != cur_set.end(); ++it) {
        cur.insert((*it).val);
    }
    
    printf("\n");
    if (cur.empty()) {// cur is empty
        printf("game: cur is empty\n");
        return false;
    }
    // cur isnt empty

    if (old.empty() && !cur.empty()) {// old is empty
        printf("game: old is empty\n");
        if (cur.size() == 2 && cur.count(16) && cur.count(17)) {// rocket
            printf("game: cur is rocket\n");
            return true;
        }
        printf("game: cur isnt rocket\n");

        if (cur.size() == 4 && has(cur, 4)) {// bomb
            printf("game: cur is bomb\n");
            return true;
        }
        printf("game: cur isnt bomb\n");

        if (cur.size() == 1) {// single
            printf("game: cur is single\n");
            return true;
        }
        printf("game: cur isnt single\n");        

        if (cur.size() == 2 && has(cur, 2)) {// dopio
            printf("game: cur is dopio\n");
            return true;
        }
        printf("game: cur isnt dopio\n");        

        if (cur.size() == 3 && has(cur, 3)) {// 3 + 0
            printf("game: cur is 3 + 0\n");
            return true;
        }
        printf("game: cur isnt 3 + 0\n");

        if (cur.size() == 4 && has(cur, 3)) {// 3 + 1
            printf("game: cur is 3 + 1\n");
            return true;
        }
        printf("game: cur isnt 3 + 1\n");

        if (cur.size() == 5 && has(cur, 3) && has(cur, 2)) {// 3 + 2
            printf("game: cur is 3 + 2\n");
            return true;
        }
        printf("game: cur isnt 3 + 2\n");

        if (cntinue(cur, 1) || cntinue(cur, 2)) {// shunzi + liandui
            printf("game: cur is shunzi or liandui\n");
            return true;
        }
        printf("game: cur isnt shunzi or liandui\n");

        if (cur.size() == 6 && has(cur, 4)) {// 4 + 2 * 1
            printf("game: cur is 4 + 2 * 1\n");
            return true;
        }
        printf("game: cur isnt 4 + 2 * 1\n");

        if (cur.size() == 8 && has(cur, 4) && dopio(cur, 2)) {// 4 + 2 * 2
            printf("game: cur is 4 + 2 * 2\n");
            return true;
        }
        printf("game: cur isnt 4 + 2 * 2\n");

        if (jet_body(cur) &&
        (cur.size() == jet_body(cur) * 3 ||
         cur.size() == jet_body(cur) * 4 ||
         cur.size() == jet_body(cur) * 5)) {// jet
            printf("game: cur is jet\n");
            return true;
        }
        printf("game: cur isnt jet\n");

        printf("game: cur is nothing\n");
        return false;
    }
    // old isnt empty

    if (old.size() == 2 && old.count(16) && old.count(17)) {// old is rocket
        printf("game: old is rocket\n");
        return false;
    }
    // old isnt rocket

    if (cur.size() == 2 && cur.count(16) && cur.count(17)) {// cur is rocket
        printf("game: cur is rocket\n");
        return true;
    }
    // cur isnt rocket
    
    if (cur.size() == 4 && has(cur, 4)) {// cur is bomb
        printf("game: cur is bomb\n");
        if (old.size() == 4 && has(old, 4)) {// old is bomb
            printf("game: old is bomb\n");
            if (*cur.begin() > *old.begin()) {
                printf("game: cur is bigger bomb\n");
                return true;
            }
            else {
                printf("game: cur is leq bomb\n");
                return false;// cur not bigger
            }
        }
        else {
            printf("game: old isnt bomb\n");
            return true;// old isnt bomb
        }
        
    }
    // cur isnt bomb
    printf("game: cur isnt bomb\n");


    if (old.size() == 4 && has(old, 4)) {// old is bomb
        printf("game: old is bomb\n");
        if (cur.size() == 4 && has(cur, 4)) {// cur is bomb
            printf("game: cur is bomb\n");
            if (*cur.begin() > *old.begin()) {
                printf("game: cur is bigger bomb\n");
                return true;
            }
            else {
                printf("game: cur is leq bomb\n");
                return false;// cur not bigger
            }
        }
        else {
            printf("game: cur isnt bomb\n");
            return false;// cur isnt bomb
        }
    }
    printf("game: old isnt bomb\n");
    // old isnt bomb

    if (old.size() == 1) {// old is single
        printf("game: old is single\n");
        if (cur.size() == 1) {// cur is single
            printf("game: cur is single\n");
            if (*cur.begin() > *old.begin()) {// cur bigger
                printf("game: cur bigger\n");
                return true;
            }
            else {
                printf("game: cur leq\n");
                return false;
            }
        }
        else {
            printf("game: cur isnt single\n");
            return false;// cur isnt single
        }
    }
    // old isnt single
    printf("game: old isnt single\n");

    if (old.size() == 2 && has(old, 2)) {// old is dopio
        printf("game: old is dopio\n");
        if (cur.size() == 2 && has(cur, 2)) {// cur is dopio
            printf("game: cur is dopio\n");
            if (*cur.begin() > *old.begin()) {// cur bigger
                printf("game: cur bigger\n");
                return true;
            }
            else {
                printf("game: cur leq\n");
                return false;
            }
        }
        else {
            printf("game: cur isnt dopio\n");
            return false;// cur isnt dopio
        }
    }
    // old isnt dopio

    if (old.size() >= 3 && old.size() <= 5 && has(old, 3)) {// old is 3 + 0, 1, 2
        printf("game: old is 3 + 0, 1, 2\n");
        if (cur.size() == old.size() && has(cur, 3)) {// cur is 3 + 0, 1, 2
            printf("game: cur is 3 + 0, 1, 2\n");
            if (has(cur, 3) > has(old, 3)) {// cur bigger
                printf("game: cur bigger\n");
                return true;
            }
            else {
                printf("game: cur leq\n");
                return false;
            }
        }
        else {
            printf("game: cur isnt 3 + 0, 1, 2\n");
            return false;// cur isnt 3 + 0, 1, 2
        }
    }
    //old isnt 3 + 0, 1, 2
    printf("game: old isnt 3 + 0, 1, 2\n");

    if (old.size() >= 5 && old.size() <= 12 && cntinue(old, 1)) {// old is shunzi 5 ~ 12
        printf("game: old is shunzi 5 ~ 12\n");
        if (cur.size() == old.size() && cntinue(cur, 1)) {// cur is shunzi 5 ~ 12
            printf("game: cur is shunzi 5 ~ 12\n");
            if (*cur.begin() > *old.begin()) {// cur is bigger
                printf("game: cur bigger\n");
                return true;
            }
            else {
                printf("game: cur leq\n");
                return false;
            }
        }
        else {
            printf("game: cur isnt shunzi 5 ~ 12\n");
            return false;// cur isnt shunzi 5 ~ 12
        }
    }
    printf("game: old isnt shunzi 5 ~ 12\n");
    // old isnt shunzi 5 ~ 12

    if (old.size() >= 6 && old.size() <= 20 && cntinue(old, 2)) {// old is liandui 6 ~ 20
        printf("game: old is liandui 6 ~ 20\n");
        if (cur.size() == old.size() && cntinue(cur, 2)) {// cur is liandui 6 ~ 20
            printf("game: cur is liandui 6 ~ 20\n");
            if (*cur.begin() > *old.begin()) {// cur is bigger
                printf("game: cur bigger\n");
                return true;
            }
            else {
                printf("game: cur leq\n");
                return false;
            }
        }
        else {
            printf("game: cur isnt liandui 6 ~ 20\n");
            return false;// cur isnt liandui 6 ~ 20
        }
    }
    printf("game: old isnt liandui 6 ~ 20\n");
    // old isnt liandui 6 ~ 20

    if (old.size() == 6 && has(old, 4)) {// old is 4 + 2 * 1
        printf("game: old is 4 + 2 * 1\n");
        if (cur.size() == 6 && has(cur, 4)) {// cur is 4 + 2 * 1
            printf("game: cur is 4 + 2 * 1\n");
            if (has(cur, 4) > has(old, 4)) {
                printf("game: cur bigger\n");
                return true;
            }
            else {
                printf("game: cur leq\n");
                return false;
            }
        }
        else {
            printf("game: cur isnt 4 + 2 * 1\n");
            return false;
        }
    }
    printf("game: old isnt 4 + 2 * 1\n");
    // old isnt 4 + 2 * 1

    if (old.size() == 8 && has(old, 4) && dopio(old, 2)) {// old is 4 + 2 * 2
        printf("game: old is 4 + 2 * 2\n");
        if (cur.size() == 8 && has(cur, 4) && dopio(cur, 2)) {// cur is 4 + 2 * 2
            printf("game: cur is 4 + 2 * 2\n");
            if (has(cur, 4) > has(old, 4)) {
                printf("game: cur bigger\n");
                return true;
            }
            else {
                printf("game: cur leq\n");
                return false;
            }
        }
        else {
            printf("game: cur isnt 4 + 2 * 2\n");
            return false;
        }
    }
    printf("game: old isnt 4 + 2 * 2\n");
    // old isnt 4 + 2 * 2

    if (jet_body(old) &&
        (old.size() == jet_body(old) * 3 ||
         old.size() == jet_body(old) * 4 ||
         old.size() == jet_body(old) * 5)) {// old is jet
        printf("game: old is jet\n");
        if (jet_body(cur) == jet_body(old) && cur.size() == old.size()) {// cur is jet
            printf("game: cur is jet\n");
            if (jet_val(cur) > jet_val(old)) {
                printf("game: cur bigger\n");                
                return true;
            }
            else {
                printf("game: cur leq\n");
                return false;
            }
        }
        else {
            printf("game: cur isnt jet\n");            
            return false;
        }
    }
    printf("game: old isnt jet\n");
    
    printf("game: FUCK!\n");
    return false;
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

string ddzgame::get_str_from_cards(set<card> cards)
{
    string ret = "";
    if (cards.empty()) return "0";
    for (set<card>::iterator it = cards.begin();
         it != cards.end(); ++it) {
        ret += to_string((*it).color) + (char)('a' + (*it).val - 1);
    }
    return ret;
}

set<card> ddzgame::get_cards_from_str(string msg) {
    set<card> cards;
    if (msg == "0") return cards;
    for (int i = 0; i < msg.length(); i += 2) {
        card cur;
        cur.color = msg[i] - '0';
        cur.val = msg[i + 1] - 'a' + 1;
        cards.insert((cur));
    }
    return cards;
}

void ddzgame::establish(int num, set<card> cards) {
    for (set<card>::iterator it = cards.begin();
         it != cards.end(); ++it) {
        player_cards[num].erase(*it);
    }
}
