#include "..\includes\framebf.h"

// from map.c
extern char map[24][32];
void drawMap(void);

static int playerX;
static int playerY;
void drawPlayer(void);

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

    // Bounds check (prevent crash when moving outside map)
    if (newX < 0 || newX >= 32 || newY < 0 || newY >= 24) return;
    // if (map[newY][newX] == 'W') return; blocked by wall, this part can be used later for some types of wall that the player cant cross

    // redraw map + player
    drawMap();
    playerX = newX;
    playerY = newY;
    drawPlayer();
}
