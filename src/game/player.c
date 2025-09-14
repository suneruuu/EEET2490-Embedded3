#include "..\includes\framebf.h"
#include "..\includes\sprites.h"
#include "map.h"
#include "player.h"

static int playerX;
static int playerY;
static char lastMove = 's';
static int animateFrame = 0;
static int animateCounter = 0;
void drawPlayer(void);

static void restoreTile(int tileX, int tileY) {
    char tile = map[tileY][tileX];
    int x = tileX * TILE_SIZE;
    int y = tileY * TILE_SIZE;

    if (tile == 'W') {
        drawRectARGB32(x, y, x + TILE_SIZE, y + TILE_SIZE, 0x000000CC, 2); // solid wall
    } else if (tile == 'P') {
        drawRectARGB32(x, y, x + TILE_SIZE, y + TILE_SIZE, 0x0000CC99, 2); // passable wall
    } else {
        drawRectARGB32(x, y, x + TILE_SIZE, y + TILE_SIZE, 0x00000000, 1); // floor
    }
}

void initPlayer(int startX, int startY) {
    playerX = startX;
    playerY = startY;
    lastMove = 's';
    animateFrame = 0;
    animateCounter = 0;
    drawPlayer();
}

void drawPlayer(void) {
    Frame f;
    if (lastMove == 'd') f = BABA_WALK_RIGHT[animateFrame];
    else if (lastMove == 'a') f = BABA_WALK_LEFT[animateFrame];
    else if (lastMove == 'w') f = BABA_WALK_UP[animateFrame];
    else f = BABA_WALK_DOWN[animateFrame];

    drawSpriteTile(f.x, f.y, TILE_SIZE, TILE_SIZE,
                   playerX * TILE_SIZE, playerY * TILE_SIZE,
                   SHEET_WIDTH);
}

void movePlayer(char input) {
    int newX = playerX;
    int newY = playerY;

    if (input == 'w' || input == 'W') { newY--; lastMove = 'w'; }
    else if (input == 's' || input == 'S') { newY++; lastMove = 's'; }
    else if (input == 'a' || input == 'A') { newX--; lastMove = 'a'; }
    else if (input == 'd' || input == 'D') { newX++; lastMove = 'd'; }

    // Bounds check
    if (newX < 0 || newX >= 32 || newY < 0 || newY >= 24) return;

    char tile = map[newY][newX];
    if (newY >= 21) return;
    // Block only solid walls
    if (tile == 'W') return;

    restoreTile(playerX, playerY);

    // Update position
    playerX = newX;
    playerY = newY;

    // Animate: flip every 4 moves
    animateCounter++;
    if (animateCounter >= 4) {
        animateCounter = 0;
        animateFrame = (animateFrame + 1) % 2;
    }

    drawPlayer();
}
