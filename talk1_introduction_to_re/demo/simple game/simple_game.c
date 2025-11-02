// minimal_dodge.c - Minimal terminal "Dodge" game for Windows
// Single-file program. Controls: A/D to move, Q to quit.

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 40
#define HEIGHT 20
#define MAX_ENEMIES 100
#define TICK_MS 100

typedef struct { int x, y, active; } Enemy;

static void set_cursor_top(void) {
    static HANDLE h = NULL;
    if (!h) h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    SetConsoleCursorPosition(h, coord);
}

int main(void) {
    srand((unsigned)time(NULL));

    int playerX = WIDTH / 2;
    int playerY = HEIGHT - 1;
    Enemy enemies[MAX_ENEMIES] = {0};
    int enemyCount = 0;
    int score = 0;
    int tick = 0;
    int spawn_chance = 5; // lower -> more frequent spawns

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    // hide cursor for nicer output
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(h, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(h, &cursorInfo);

    // main loop
    for (;;) {
        // input (non-blocking)
        if (_kbhit()) {
            int c = _getch();
            if (c == 'a' || c == 'A') if (playerX > 0) playerX--;
            if (c == 'd' || c == 'D') if (playerX < WIDTH - 1) playerX++;
            if (c == 'q' || c == 'Q') break;
        }

        // spawn enemy occasionally
        if ((rand() % spawn_chance) == 0) {
            for (int i = 0; i < MAX_ENEMIES; ++i) {
                if (!enemies[i].active) {
                    enemies[i].active = 1;
                    enemies[i].x = rand() % WIDTH;
                    enemies[i].y = 0;
                    enemyCount++;
                    break;
                }
            }
        }

        // move enemies down
        for (int i = 0; i < MAX_ENEMIES; ++i) {
            if (enemies[i].active) {
                enemies[i].y++;
                if (enemies[i].y >= HEIGHT) {
                    enemies[i].active = 0;
                    enemyCount--;
                    score++;
                }
            }
        }

        // collision check
        for (int i = 0; i < MAX_ENEMIES; ++i) {
            if (enemies[i].active && enemies[i].y == playerY && enemies[i].x == playerX) {
                set_cursor_top();
                printf("GAME OVER! Score: %d\n", score);
                printf("Press any key to exit...\n");
                while (!_kbhit()) Sleep(10);
                // restore cursor visibility
                cursorInfo.bVisible = TRUE;
                SetConsoleCursorInfo(h, &cursorInfo);
                return 0;
            }
        }

        // draw to a simple buffer
        static char buf[HEIGHT][WIDTH + 1];
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) buf[y][x] = ' ';
            buf[y][WIDTH] = '\0';
        }

        // enemies
        for (int i = 0; i < MAX_ENEMIES; ++i) {
            if (enemies[i].active && enemies[i].y >= 0 && enemies[i].y < HEIGHT) {
                buf[enemies[i].y][enemies[i].x] = 'O';
            }
        }

        // player
        buf[playerY][playerX] = 'A';

        // print screen (move cursor to top-left first to avoid flicker)
        set_cursor_top();
        for (int y = 0; y < HEIGHT; ++y) {
            printf("|%s|\n", buf[y]);
        }
        printf("Score: %d  (A/D to move, Q to quit)\n", score);

        Sleep(TICK_MS);
        tick++;

        // slowly increase difficulty
        if (tick % 200 == 0 && spawn_chance > 3) spawn_chance--;
    }

    // restore cursor visibility before exiting
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(h, &cursorInfo);
    return 0;
}
