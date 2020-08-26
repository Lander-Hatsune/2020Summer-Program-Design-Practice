#pragma once
#include <cstdio>
#include <queue>
#include <cstring>
#include <random>
#include <time.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif // _WIN32
using namespace std;


const int GRID_SIZE = 40;

typedef struct coordinate {
    int x, y;
    struct coordinate operator + (const struct coordinate& a) {
        return (struct coordinate){x + a.x, y + a.y};
    }
}cord;

const cord DIR_CORD[5] = {{0, 0}, {0, -1}, {-1, 0}, {0, 1}, {1, 0}};

class Snakegame {
public:
    static void sleepcp(int);
    Snakegame();
    ~Snakegame();
    int move();
    void change_dir(int new_dir);
    const queue<cord>* game_snake;
    int arena[45][45];
    queue<cord> snake;
    void save(const char* path);
    void load(const char* path);
    int score;
    
private:
    int stretch;
    int dir;
};
