#include "..\includes\framebf.h"
#include "..\includes\sprites.h"
#include "map.h"
#include "player.h"

static int playerX;
static int playerY;
static char lastMove = 's';
static int animateFrame = 0;
static int animateCounter = 0;
static int isMoving = 0;  

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
    isMoving = 0;
    drawPlayer();
}

void drawPlayer(void) {
    Frame f;

    if (!isMoving) {
        // Show idle frame if not moving
        f = BABA_IDLE;
    } else {
        // select walk frame set based on lastMove
        if (lastMove == 'd' || lastMove == 'D') {
            f = BABA_WALK_RIGHT[animateFrame % 2];
        } else if (lastMove == 'a' || lastMove == 'A') {
            f = BABA_WALK_LEFT[animateFrame % 2];
        } else if (lastMove == 'w' || lastMove == 'W') {
            f = BABA_WALK_UP[animateFrame % 2];
        } else {
            f = BABA_WALK_DOWN[animateFrame % 2];
        }
    }

    // Draw using ARGB sprite drawing function that respects alpha
    drawSpriteARGB32(f.data, f.width, f.height, playerX * TILE_SIZE, playerY * TILE_SIZE);
}

void movePlayer(char input) {
    int newX = playerX;
    int newY = playerY;
    isMoving = 0; // reset each frame

    if (input == 'w' || input == 'W') { newY--; lastMove = 'w'; isMoving = 1; }
    else if (input == 's' || input == 'S') { newY++; lastMove = 's'; isMoving = 1; }
    else if (input == 'a' || input == 'A') { newX--; lastMove = 'a'; isMoving = 1; }
    else if (input == 'd' || input == 'D') { newX++; lastMove = 'd'; isMoving = 1; }
    else {
        // Not a movement key → stay idle
        drawPlayer();
        return;
    }

    // Bounds check (map width 32, height 24)
    if (newX < 0 || newX >= 32 || newY < 0 || newY >= 24) return;

    // Safety: disallow movement into bottom UI region
    if (newY >= 21) return;

    char tile = map[newY][newX];

    // Block only solid walls
    if (tile == 'W') return;

    // Restore the tile we are leaving
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

    // Draw at the new position
    drawPlayer();
}
