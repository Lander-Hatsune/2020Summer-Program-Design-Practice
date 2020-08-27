#include "game.h"
using namespace std;

void Snakegame::sleepcp(int milliseconds) // 跨平台 sleep 函数
{
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif // _WIN32
}

Snakegame::Snakegame() {
    srand(time(NULL));
    memset(arena, 0, sizeof(arena));
    this->dir = 3;
    this->stretch = 0;
    this->game_snake = &this->snake;
    snake.push((cord){4, 2});
    snake.push((cord){4, 3});
    arena[4][3] = arena[4][2] = 2;
    for (int i = 1; i <= GRID_SIZE; i++) {
        arena[1][i] = arena[GRID_SIZE][i] = 3;
        arena[i][1] = arena[i][GRID_SIZE] = 3;
    }
    score = 0;
}

Snakegame::~Snakegame() {
    ;
}

int Snakegame::move() {
    score += 1;
    bool eaten = false;
    cord head = this->snake.back();
    head = head + DIR_CORD[this->dir];
    int cur_terrain = arena[head.x][head.y];
    if (cur_terrain != 0 && cur_terrain != 1) {
        printf("headx: %d, heady: %d, terr: %d\n", head.x, head.y, cur_terrain);
        return -1;
    }
    this->snake.push(head);
    arena[head.x][head.y] = 2;// snake body
    if (cur_terrain == 1) {// fruit
        this->stretch += 3;
        eaten = true;
    }
    if (this->stretch == 0) {
        cord tail = snake.front();
        this->snake.pop();
        arena[tail.x][tail.y] = 0;// empty
    } else {
        this->stretch -= 1;
    }
    
    if (eaten) {
        int fx = (rand() % GRID_SIZE) + 1;
        int fy = (rand() % GRID_SIZE) + 1;
        while (arena[fx][fy] != 0) {
            fx = (rand() % GRID_SIZE) + 1,
            fy = (rand() % GRID_SIZE) + 1;
        }
        arena[fx][fy] = 1;
    }
    return 0;
}

void Snakegame::change_dir(int new_dir) {
    if (abs(new_dir - this->dir) == 2)
        return;
    if (new_dir == this->dir)
        return;
    this->dir = new_dir;
    sleepcp(60);
}

void Snakegame::save(const char* path) {
    freopen(path, "w", stdout);
    printf("%d %d %d\n", stretch, dir, score);
    printf("%d\n", snake.size());
    int snake_size = snake.size();
    for (int i = 1; i <= snake_size; i++) {
        cord cur_cord = snake.front();
        snake.pop();
        snake.push(cur_cord);
        printf("%d %d\n", cur_cord.x, cur_cord.y);
    }
    for (int i = 1; i <= GRID_SIZE; i++) {
        for (int j = 1; j <= GRID_SIZE; j++) {
            printf("%d ", arena[i][j]);
        }
        putchar('\n');
    }
    freopen("CON", "w", stdout);
}

void Snakegame::load(const char* path) {
    freopen(path, "r", stdin);
    try {
        scanf("%d %d %d", &stretch, &dir, &score);
        int snake_size;
        scanf("%d", &snake_size);

        while (!snake.empty()) {
            snake.pop();
        }

        for (int i = 1; i <= snake_size; i++) {
            cord cur_cord;
            scanf("%d %d", &cur_cord.x, &cur_cord.y);
            snake.push(cur_cord);
        }

        for (int i = 1; i <= GRID_SIZE; i++) {
            for (int j = 1; j <= GRID_SIZE; j++) {
                scanf("%d ", &arena[i][j]);
            }
        }
    }  catch (...) {
        printf("Invalid file\n");
    }

    freopen("CON", "r", stdout);
}
