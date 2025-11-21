/* adventure_chal.c
   Tiny 2D terminal adventure — reverse me!
   Compile: gcc -O2 -static -s -o adventure_chal adventure_chal.c
   (static build reduces reliance on external libs and is friendly for CTF distribution)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Map size */
#define W 20
#define H 8

/* Simple map ('.' floor, '#' wall) */
static const char *base_map[H] = {
    "####################",
    "#..G....#....#....T#",
    "#..##...#..##.#....#",
    "#......###....#..#.#",
    "#..##..............#",
    "#..##...##..##..#..#",
    "#P....#....#..B....#",
    "####################"
};

/* Game objects legend:
   P - starting player position
   T - treasure (win condition requires specific preconditions)
   G - glowing rock (gives "gem" when 'pickup' key used while adjacent)
   B - brittle wall (must be broken with 'use' while adjacent AND player must have torch lit)
   Walls '#' block movement.
*/

/* Obfuscated keybinds (constructed in code so they don't appear as plain strings)
   The intention: solver must inspect the binary to find how keybinds are computed.
*/
struct binds { char up, down, left, right, pickup, useitem, light; };

static struct binds compute_binds(void) {
    /* Compute binds using a mix of arithmetic and bitwise ops.
       Static analysis will allow the reverse-engineer to reconstruct these constants.
    */
    struct binds b;
    int a = 0x2E;            /* 46 */
    int t = (a ^ 0x14) + 3;  /* ((46 ^ 20)=58) +3 = 61 -> '=' (not used; just noise) */
    /* Real binds (computed from hidden math) */
    b.up     = (char)('k' - 5);          /* 'f' (102) */
    b.down   = (char)('p' - 8);                        /* 'h' (104) */
    b.left   = (char)('z' - 7);                        /* 's' (115) */
    b.right  = (char)('|' - 18);                       /* 'i' (105) */
    b.pickup = (char)((0x30) ^ 0x1F);                  /* '!' (33) */
    b.useitem= (char)((0x61 - 1) ^ 0x0);               /* 'a' (97) */
    b.light  = (char)(('m' + 1) - 3);                  /* 'k' (107) */
    /* A small red herring variable referencing t so compiler doesn't optimize it entirely away */
    if (t == 9999) b.up = '?';
    return b;
}

/* Simple player state */
struct player {
    int x, y;
    int has_gem;
    int torch_lit;
    int broke_wall;
};

/* Print map with player overlay */
static void draw_map(const char *map[], struct player *p) {
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            char c = map[y][x];
            if (x == p->x && y == p->y) putchar('@');
            else putchar(c);
        }
        putchar('\n');
    }
    printf("State: gem=%d torch=%d wall_broken=%d\n", p->has_gem, p->torch_lit, p->broke_wall);
}

/* Find coordinates of a char in map (first occurrence) */
static int find_char(const char *map[], char ch, int *rx, int *ry) {
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
            if (map[y][x] == ch) {
                *rx = x; *ry = y; return 1;
            }
    return 0;
}

/* Safe map access */
static char map_at(const char *map[], int x, int y) {
    if (x < 0 || x >= W || y < 0 || y >= H) return '#';
    return map[y][x];
}

/* Check if position is walkable */
static int walkable(const char *map[], int x, int y) {
    char c = map_at(map, x, y);
    return c != '#';
}

/* Main game logic (no textual help about binds) */
int main(void) {
    struct binds kb = compute_binds();
    const char **map = base_map;
    struct player pl = {0};
    int px, py;
    if (!find_char(map, 'P', &px, &py)) return 1;
    pl.x = px; pl.y = py;
    pl.has_gem = 0; pl.torch_lit = 0; pl.broke_wall = 0;

    /* Remove P from map view (map is const, so we won't mutate it; treat P as floor) */
    char input = 0;
    int ch;
    /* Game loop: each iteration draws the map and waits for a single character + newline */
    while (1) {
        draw_map(map, &pl);
        printf("Enter command: ");
        fflush(stdout);

        ch = getchar();
        if (ch == EOF) break;
        /* consume rest of line */
        while (ch != '\n' && ch != EOF) {
            input = (char)ch;
            /* process the char */
            if (input == kb.up) {
                if (walkable(map, pl.x, pl.y-1)) pl.y--;
            } else if (input == kb.down) {
                if (walkable(map, pl.x, pl.y+1)) pl.y++;
            } else if (input == kb.left) {
                if (walkable(map, pl.x-1, pl.y)) pl.x--;
            } else if (input == kb.right) {
                if (walkable(map, pl.x+1, pl.y)) pl.x++;
            } else if (input == kb.pickup) {
                /* If adjacent to G (glowing rock), pick up gem */
                int found = 0;
                for (int dy = -1; dy <= 1 && !found; ++dy)
                for (int dx = -1; dx <= 1 && !found; ++dx) {
                    if (map_at(map, pl.x+dx, pl.y+dy) == 'G') {
                        pl.has_gem = 1; found = 1;
                    }
                }
            } else if (input == kb.light) {
                /* toggle torch (light) */
                pl.torch_lit = !pl.torch_lit;
            } else if (input == kb.useitem) {
                /* If adjacent to B (brittle wall) and torch lit, break it */
                for (int dy = -1; dy <= 1; ++dy)
                for (int dx = -1; dx <= 1; ++dx) {
                    if (map_at(map, pl.x+dx, pl.y+dy) == 'B' && pl.torch_lit) {
                        pl.broke_wall = 1;
                    }
                }
            } else if (input == 'q') {
                puts("Goodbye.");
                return 0;
            } else {
                /* ignore unknown key */
            }

            /* after processing, check win condition: player on T and has gem and wall broken */
            if (map_at(map, pl.x, pl.y) == 'T' && pl.has_gem && pl.broke_wall) {
                puts("\nYou found the TREASURE!");
                puts("WIN!");
                return 0;
            }

            /* consume until newline */
            ch = getchar();
        }
    }

    return 0;
}

