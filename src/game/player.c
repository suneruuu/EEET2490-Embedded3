#include "../../includes/framebf.h"
#include "../../includes/sprites.h"
#include "../../includes/game/map.h"
#include "../../includes/game/player.h"
#include "../../includes/game/game_logic.h"

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
    
    // Initialize game logic
    initGameLogic();
    setPlayerPosition(startX, startY);
    parseRules();
    
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
    int dirX = 0, dirY = 0;
    isMoving = 0; // reset each frame

    if (input == 'w' || input == 'W') { dirY = -1; lastMove = 'w'; isMoving = 1; }
    else if (input == 's' || input == 'S') { dirY = 1; lastMove = 's'; isMoving = 1; }
    else if (input == 'a' || input == 'A') { dirX = -1; lastMove = 'a'; isMoving = 1; }
    else if (input == 'd' || input == 'D') { dirX = 1; lastMove = 'd'; isMoving = 1; }
    else {
        // Not a movement key → stay idle
        drawPlayer();
        return;
    }

    // Check if player can move to the new position
    int newX = playerX + dirX;
    int newY = playerY + dirY;
    
    // Check if movement is allowed
    if (!canMoveToAsYou(newX, newY)) {
        char tile = map[newY][newX];
        
        // Check if we can push a text block (always pushable)
        if (isTextBlock(tile)) {
            // Calculate push destination
            int pushX = newX + (newX - playerX);
            int pushY = newY + (newY - playerY);
            
            if (canPushTextBlock(newX, newY, pushX, pushY)) {
                // Push the text block
                pushObject(newX, newY, pushX, pushY);
                
                // Restore the tile we are leaving
                restoreTile(playerX, playerY);
                
                // Update position
                playerX = newX;
                playerY = newY;
                setPlayerPosition(newX, newY);
                
                // Animate: flip every 4 moves
                animateCounter++;
                if (animateCounter >= 4) {
                    animateCounter = 0;
                    animateFrame = (animateFrame + 1) % 2;
                }
                
                // Draw at the new position
                drawPlayer();
                
                // Handle collisions (DEFEAT, SINK)
                handleCollision(newX, newY);
                
                // Update game logic
                updateGameLogic();
            }
        }
        // Check if we can push an image block (only if rules allow)
        else if (isImageBlock(tile) && isPushable(newX, newY)) {
            // Calculate push destination
            int pushX = newX + (newX - playerX);
            int pushY = newY + (newY - playerY);
            
            if (canPushTextBlock(newX, newY, pushX, pushY)) {
                // Push the object
                pushObject(newX, newY, pushX, pushY);
                
                // Restore the tile we are leaving
                restoreTile(playerX, playerY);
                
                // Update position
                playerX = newX;
                playerY = newY;
                setPlayerPosition(newX, newY);
                
                // Animate: flip every 4 moves
                animateCounter++;
                if (animateCounter >= 4) {
                    animateCounter = 0;
                    animateFrame = (animateFrame + 1) % 2;
                }
                
                // Draw at the new position
                drawPlayer();
                
                // Handle collisions (DEFEAT, SINK)
                handleCollision(newX, newY);
                
                // Update game logic
                updateGameLogic();
            }
        }
        return;
    }

    // Restore the tile we are leaving
    restoreTile(playerX, playerY);

    // Update position
    playerX = newX;
    playerY = newY;
    setPlayerPosition(newX, newY);

    // Animate: flip every 4 moves
    animateCounter++;
    if (animateCounter >= 4) {
        animateCounter = 0;
        animateFrame = (animateFrame + 1) % 2;
    }

    // Draw at the new position
    drawPlayer();
    
    // Handle collisions (DEFEAT, SINK)
    handleCollision(newX, newY);
    
    // Update game logic
    updateGameLogic();
}
