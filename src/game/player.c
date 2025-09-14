#include "framebf.h"

// from map.c
extern char map[24][32];
void drawMap(void);

static int playerX;
static int playerY;

void initPlayer(int startX, int startY)
{
    playerX = startX;
    playerY = startY;
    drawPlayer();
}

void drawPlayer(void)
{
    int x = playerX * 25;
    int y = playerY * 25;
    drawRectARGB32(x, y, x + 25, y + 25, 0xFFFFFFFF, 2); // White square = Baba
}

void movePlayer(char input)
{
    int newX = playerX;
    int newY = playerY;

    if (input == 'w' || input == 'W') newY--;
    else if (input == 's' || input == 'S') newY++;
    else if (input == 'a' || input == 'A') newX--;
    else if (input == 'd' || input == 'D') newX++;

    if (map[newY][newX] == 'W') return; // blocked by wall

    // redraw map + player
    drawMap();
    playerX = newX;
    playerY = newY;
    drawPlayer();
}
