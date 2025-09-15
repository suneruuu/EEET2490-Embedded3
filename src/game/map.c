#include "../../includes/framebf.h"
#include "map.h"

// Define size_t manually for bare-metal
typedef unsigned long size_t;

void *map_memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

void *memset(void *s, int c, unsigned long n) {
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

char map[24][32]; // accessible from player.c

void LoadLevel1(void) {
    char tmp[24][32] = {
        "                                ",
        "   WWWWWWWWWWWWWWWWWWWWWWWWWW   ",
        "   W        P                 W",
        "   W   WWW      WWWW   WWW    W",
        "   W   W   W              W   W",
        "   W   W   WWWW   WWWW    W   W",
        "   W   W                  W   W",
        "   W   WWWW   WW   WWWW   W   W",
        "   W                        F W",
        "   W   W   WWWW   WWWW    W   W",
        "   W   W                  W   W",
        "   W   W   W              W   W",
        "   W   WWW      WWWW   WWW    W",
        "   W                        W W",
        "   WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
        "                                ",
        "                                ",
        "                                ",
        "                                ", 
        "                                ", 
        "                                ",
        "                                ",
        "                                ",
        "                                ",
    };
    map_memcpy(map, tmp, sizeof(tmp));
}

void LoadLevel2(void) {
    char tmp[24][32] = {
        "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
        "W      B                     W",
        "W   WWWWW                    W",
        "W   W   W                    W",
        "W P W   W         F          W",
        "W   W   W                    W",
        "W   W   WWWWWW               W",
        "W   W                        W",
        "W   W                        W",
        "W                            W",
        "W                            W",
        "W                            W",
        "W                            W",
        "W                            W",
        "W                            W",
        "W                            W",
        "W                            W",
        "W                            W",
        "W                            W",
        "W                            W",
        "W                            W",
        "W                            W",
        "W                            W",
        "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    };
    map_memcpy(map, tmp, sizeof(tmp));
}

void drawMap(void) {
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 32; j++) {
            int x = j * TILE_SIZE;
            int y = i * TILE_SIZE;

            if (map[i][j] == 'W') {
                drawRectARGB32(x, y, x + TILE_SIZE, y + TILE_SIZE, 0x000000CC, 2); // solid wall
            } else if (map[i][j] == 'P') {
                drawRectARGB32(x, y, x + TILE_SIZE, y + TILE_SIZE, 0x0000CC99, 2); // passable wall (different shade)
            } else {
                drawRectARGB32(x, y, x + TILE_SIZE, y + TILE_SIZE, 0x00000000, 1); // background/floor
            }
        }
    }

    for (int x = 0; x < 800; x++) {
        drawPixelARGB32(x, 550, 0xFFFFFFFF);  // 1px horizontal white line
    }

    drawString(20, 560, "     WASD: Move  |  B: Back  |  Q: Main Menu", 0xFFFFFF, 2);
}

