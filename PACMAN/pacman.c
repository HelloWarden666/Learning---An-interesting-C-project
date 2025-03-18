// C 语言的 Pacman 游戏 
#include <conio.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h>
#include <windows.h>
#include <time.h>
#include <math.h>

// 前向声明
void initialize();
void draw();
void move_pacman(int dx, int dy);
void move_demonA();
void move_demonM();

// 游戏常量
#define WIDTH 40
#define HEIGHT 20
#define PACMAN 'C'
#define WALL '#'  // ▒
#define FOOD '.'  // •
#define DEMON_A 'A'
#define DEMON_M 'M'
#define EMPTY ' '

// 全局状态
int game_over = 0;
int score = 0;
int pacman_x, pacman_y;
int demonA_x, demonA_y;
int demonM_x, demonM_y;
wchar_t board[HEIGHT][WIDTH];
int total_food = 0;
int eaten_food = 0;

// 计算距离
double distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// 初始化游戏
void initialize() {
    // 设置控制台编码
    SetConsoleOutputCP(65001);
    
    // 初始化地图
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            board[i][j] = (i == 0 || j == WIDTH-1 || j == 0 || i == HEIGHT-1) ? WALL : EMPTY;
        }
    }

    // 添加随机墙
    for (int count = 0; count < 50; ) {
        int i = rand() % (HEIGHT-2) + 1;
        int j = rand() % (WIDTH-2) + 1;
        if (board[i][j] == EMPTY) {
            board[i][j] = WALL;
            count++;
        }
    }

    // 放置恶魔
    const wchar_t demons[] = { DEMON_A, DEMON_M };
    int demon_pos[2][2] = {{0}};
    for (int d = 0; d < 2; d++) {
        int attempts = 100;
        while (attempts--) {
            int i = rand() % (HEIGHT-2) + 1;
            int j = rand() % (WIDTH-2) + 1;
            if (board[i][j] == EMPTY) {
                board[i][j] = demons[d];
                demon_pos[d][0] = j;
                demon_pos[d][1] = i;
                break;
            }
        }
    }
    demonA_x = demon_pos[0][0];
    demonA_y = demon_pos[0][1];
    demonM_x = demon_pos[1][0];
    demonM_y = demon_pos[1][1];

    // 生成食物
    for (int i = 1; i < HEIGHT-1; i++) {
        for (int j = 1; j < WIDTH-1; j++) {
            if ((i+j) % 3 == 0 && board[i][j] == EMPTY) {
                board[i][j] = FOOD;
                total_food++;
            }
        }
    }

    // 玩家初始位置
    pacman_x = WIDTH / 2;
    pacman_y = HEIGHT / 2;
    board[pacman_y][pacman_x] = PACMAN;
}

// 绘制界面
void draw() {
    system("cls");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            putwchar(board[i][j]);
        }
        putwchar(L'\n');
    }
    wprintf(L"得分: %d  剩余食物: %d\n", score, total_food - eaten_food);
}

// 玩家移动
void move_pacman(int dx, int dy) {
    int new_x = pacman_x + dx;
    int new_y = pacman_y + dy;

    if (new_x < 1 || new_x >= WIDTH-1 || new_y < 1 || new_y >= HEIGHT-1)
        return;

    wchar_t target = board[new_y][new_x];
    
    if (target == WALL) return;
    
    if (target == FOOD) {
        score += 10;
        eaten_food++;
    }

    board[pacman_y][pacman_x] = EMPTY;
    pacman_x = new_x;
    pacman_y = new_y;
    board[pacman_y][pacman_x] = PACMAN;

    if (eaten_food >= total_food) game_over = 2;
}

// 恶魔A移动逻辑
void move_demonA() {
    if (eaten_food < 3) return;

    int dx = 0, dy = 0;
    double dist = distance(pacman_x, pacman_y, demonA_x, demonA_y);

    if (dist <= 3.0) {
        dx = (demonA_x < pacman_x) ? -1 : (demonA_x > pacman_x) ? 1 : 0;
        dy = (demonA_y < pacman_y) ? -1 : (demonA_y > pacman_y) ? 1 : 0;
    } else {
        dx = (demonA_x > pacman_x) ? -1 : (demonA_x < pacman_x) ? 1 : 0;
        dy = (demonA_y > pacman_y) ? -1 : (demonA_y < pacman_y) ? 1 : 0;
    }

    int new_x = demonA_x + dx;
    int new_y = demonA_y;
    if (board[new_y][new_x] == EMPTY) {
        board[demonA_y][demonA_x] = EMPTY;
        demonA_x = new_x;
        board[demonA_y][demonA_x] = DEMON_A;
        return;
    }

    new_x = demonA_x;
    new_y = demonA_y + dy;
    if (board[new_y][new_x] == EMPTY) {
        board[demonA_y][demonA_x] = EMPTY;
        demonA_y = new_y;
        board[demonA_y][demonA_x] = DEMON_A;
    }
}

// 恶魔M移动逻辑
void move_demonM() {
    int dx = (demonM_x > pacman_x) ? -1 : (demonM_x < pacman_x) ? 1 : 0;
    int dy = (demonM_y > pacman_y) ? -1 : (demonM_y < pacman_y) ? 1 : 0;

    int new_x = demonM_x + dx;
    int new_y = demonM_y;
    if (board[new_y][new_x] == EMPTY) {
        board[demonM_y][demonM_x] = EMPTY;
        demonM_x = new_x;
        board[demonM_y][demonM_x] = DEMON_M;
        return;
    }

    new_x = demonM_x;
    new_y = demonM_y + dy;
    if (board[new_y][new_x] == EMPTY) {
        board[demonM_y][demonM_x] = EMPTY;
        demonM_y = new_y;
        board[demonM_y][demonM_x] = DEMON_M;
    }
}

int main() {
    srand((unsigned int)time(NULL));
    initialize();

    wprintf(L"欢迎来到PAC-MAN!\n操作说明：\n");
    wprintf(L"WASD/方向键：移动\nQ：退出游戏\n准备好开始了吗？(Y/N)");
    
    if (toupper(_getwch()) != L'Y') {
        wprintf(L"\n游戏退出！");
        return 0;
    }

    while (!game_over) {
        draw();
        
        // 处理输入
        if (_kbhit()) {
            int ch = _getwch();
            if (ch == 0xE0 || ch == 0) {
                ch = _getwch();
                switch (ch) {
                    case 72: move_pacman(0, -1); break;
                    case 80: move_pacman(0, 1);  break;
                    case 75: move_pacman(-1, 0); break;
                    case 77: move_pacman(1, 0);  break;
                }
            } else {
                switch (towlower(ch)) {
                    case L'w': move_pacman(0, -1); break;
                    case L's': move_pacman(0, 1);  break;
                    case L'a': move_pacman(-1, 0); break;
                    case L'd': move_pacman(1, 0);  break;
                    case L'q': game_over = 3; break;
                }
            }
        }

        // 恶魔移动
        move_demonA();
        move_demonM();

        // 碰撞检测
        if ((demonA_x == pacman_x && demonA_y == pacman_y) ||
            (demonM_x == pacman_x && demonM_y == pacman_y)) {
            game_over = 1;
        }

        Sleep(50);
    }

    system("cls");
    if (game_over == 1) {
        wprintf(L"游戏结束！被恶魔抓住了！\n最终得分：%d", score);
    } else if (game_over == 2) {
        wprintf(L"恭喜！吃掉了所有食物！\n最终得分：%d", score);
    } else {
        wprintf(L"游戏已退出，当前得分：%d", score);
    }
    
    getchar();
    return 0;
}
