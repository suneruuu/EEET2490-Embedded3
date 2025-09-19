#include "../../includes/framebf.h"
#include "../../includes/game/map.h"
#include "../../includes/game/game_logic.h"
#include "../../includes/timer.h"
#include "../../assets/images/wall1.c"
#include "../../assets/images/wall2.c"
#include "../../assets/images/wall3.c"
#include "../../assets/images/road.c"
#include "../../assets/images/baba.c"
#include "../../assets/images/is.c"
#include "../../assets/images/you.c"
#include "../../assets/images/flag_txt.c"
#include "../../assets/images/win.c"
#include "../../assets/images/wall_txt.c"
#include "../../assets/images/stop.c"
#include "../../assets/images/rock_txt.c"
#include "../../assets/images/push.c"
#include "../../assets/images/rock.c"
#include "../../assets/images/flag.c"

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
static char stepBuf[32];
static char timeBuf[32];

// Convert integer to decimal string (safe even if no font for digits)
static void intToStr(int value, char *buf) {
    if (value == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    char tmp[16];
    int i = 0;
    while (value > 0) {
        tmp[i++] = '0' + (value % 10);
        value /= 10;
    }
    // reverse into buf
    int j = 0;
    while (i > 0) {
        buf[j++] = tmp[--i];
    }
    buf[j] = '\0';
}

void drawHUD(void) {
    // Steps
    intToStr(getStepCounter(), stepBuf);
    drawRectARGB32(0, 0, 799, 24, 0xFF000000, 1); // clear top HUD bar
    drawString(10, 5, "Steps:", 0xFFFFFFFF, 2);
    drawString(100, 5, stepBuf, 0xFFFFFFFF, 2);

    // Time
    intToStr(getElapsedSeconds(), timeBuf);
    drawString(200, 5, "Time:", 0xFFFFFFFF, 2);
    drawString(280, 5, timeBuf, 0xFFFFFFFF, 2);
}


void LoadLevel1(void) {
    char tmp[MAP_HEIGHT][MAP_WIDTH] = {
        "                                ",
        "                                ",
        "  abc                    dbe    ",
        "                                ",
        " QWWWWWWWWWWWWWWWWWWWWWWWWWWWWE ",
        " FFFFFFFFFFFFFFRFFFFFFFFFFFFFFF ",
        " FFFFFFFFFFFFFFRFFFFFFFFFFFFFFF ",
        " FFFFFFFFFFFFFFRFFFFFFFFFFFFFFF ",
        " FFFFFFFFFFFFFFRFFFFFFFZFFFFFFF ",
        " FFFFFFFFFFFFFFRFFFFFFFFFFFFFFF ",
        " FFFFFFFFFFFFFFRFFFFFFFFFFFFFFF ",
        " FFFFFFFFFFFFFFRFFFFFFFFFFFFFFF ",
        " QWWWWWWWWWWWWWWWWWWWWWWWWWWWWE ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "hbi                             ",
        "gbf                             ",
        "                                ",
        "                                ",
    };
    map_memcpy(map, tmp, sizeof(map));
    resetTimer();
}

void LoadLevel2(void) {
    char tmp[MAP_HEIGHT][MAP_WIDTH] = {
        "                                ",
        "                                ",
        "        QWWWWWWWWWWWWWWWWWWWWWE ",
        "        W                     W ",
        "        W   a                 W ",
        "        W   b                 W ",
        "        W   c                 W ",
        "        W                     W ",
        " QWWWWWWW            Z        W ",
        " WFFFFFFF                     W ",
        " WFFdFFFF          be         W ",
        " WFFFFFFF                     W ",
        " QWWWWWWWWWWWWWWWWWWWWWWWWWWWWE ",
        "           W                  W ",
        "           W   g              W ",
        "           W   b              W ",
        "           W   f              W ",
        "           W                  W ",
        "           QWWWWWWWWWWWWWWWWWWE ",
        "h                               ",
        "b                               ",
        "f                               ",
        "                                ",
        "                                "
    };
    map_memcpy(map, tmp, sizeof(map));
    resetTimer();
}

void drawSpriteARGB32Scaled(const unsigned long *sprite, int srcW, int srcH,
                            int dstW, int dstH, int dstX, int dstY) {
    for (int y = 0; y < dstH; y++) {
        int sy = (y * srcH) / dstH;
        for (int x = 0; x < dstW; x++) {
            int sx = (x * srcW) / dstW;
            unsigned long color = sprite[sy * srcW + sx];

            unsigned char r = (color >> 24) & 0xFF;
            unsigned char g = (color >> 16) & 0xFF;
            unsigned char b = (color >>  8) & 0xFF;
            unsigned char a =  color        & 0xFF;

            if (a == 0) continue; // skip fully transparent

            // Skip "outline/background" pixels that are blue-dominant
            if (b > r + 20 && b > g + 20) {
                continue;
            }
            // Otherwise draw normally
            drawPixelARGB32(dstX + x, dstY + y, color);
        }
    }
}

void drawMap(void) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            int x = j * TILE_SIZE;
            int y = i * TILE_SIZE;
            char c = map[i][j];

            switch (c) {
                case 'Q': // left wall
                    drawSpriteARGB32Scaled(epd_bitmap_tile003, TILE_SIZE, TILE_SIZE,
                                           TILE_SIZE, TILE_SIZE, x, y);
                    break;

                case 'W': // middle wall
                    drawSpriteARGB32Scaled(epd_bitmap_tile007, TILE_SIZE, TILE_SIZE,
                                           TILE_SIZE, TILE_SIZE, x, y);
                    break;

                case 'E': // right wall
                    drawSpriteARGB32Scaled(epd_bitmap_tile006, TILE_SIZE, TILE_SIZE,
                                           TILE_SIZE, TILE_SIZE, x, y);
                    break;

                case 'F': //floor or tiles
                    drawSpriteARGB32Scaled(road_tile, TILE_SIZE, TILE_SIZE,
                                           TILE_SIZE, TILE_SIZE, x, y);
                    break;

                case 'R': //rock
                    drawSpriteARGB32Scaled(epd_bitmap_rock, TILE_SIZE, TILE_SIZE,
                                           TILE_SIZE, TILE_SIZE, x, y);
                    break;

                case 'Z': //flag
                    drawSpriteARGB32Scaled(epd_bitmap_flag, TILE_SIZE, TILE_SIZE,
                                           TILE_SIZE, TILE_SIZE, x, y);
                    break;

                case 'a': //baba text
                {
                    // Don't draw BABA sprite if player is at this position
                    // (player sprite will be drawn instead)
                    int playerX, playerY;
                    getPlayerPosition(&playerX, &playerY);
                    if (x / TILE_SIZE != playerX || y / TILE_SIZE != playerY) {
                        drawSpriteARGB32Scaled(epd_bitmap_baba, TILE_SIZE, TILE_SIZE,
                                               TILE_SIZE, TILE_SIZE, x, y);
                    }
                    break;
                }

                case 'b': //is text
                    drawSpriteARGB32Scaled(epd_bitmap_is, TILE_SIZE, TILE_SIZE,
                                           TILE_SIZE, TILE_SIZE, x, y);
                    break;

                case 'c': //you text
                    drawSpriteARGB32Scaled(epd_bitmap_you, TILE_SIZE, TILE_SIZE,
                                           TILE_SIZE, TILE_SIZE, x, y);
                    break;

                case 'd': //flag text
                    drawSpriteARGB32Scaled(epd_bitmap_flag_text, TILE_SIZE, TILE_SIZE,
                                           TILE_SIZE, TILE_SIZE, x, y);
                    break;

                case 'e': //win text
                    drawSpriteARGB32Scaled(epd_bitmap_win, TILE_SIZE, TILE_SIZE,
                                           TILE_SIZE, TILE_SIZE, x, y);
                    break;

                case 'f': //stop text
                    drawSpriteARGB32Scaled(epd_bitmap_stop, TILE_SIZE, TILE_SIZE,
                                           TILE_SIZE, TILE_SIZE, x, y);
                    break;

                case 'g': //wall text
                    drawSpriteARGB32Scaled(epd_bitmap_wall, TILE_SIZE, TILE_SIZE,
                                           TILE_SIZE, TILE_SIZE, x, y);
                    break;

                case 'h': //rock text
                    drawSpriteARGB32Scaled(epd_bitmap_rock_text, TILE_SIZE, TILE_SIZE,
                                           TILE_SIZE, TILE_SIZE, x, y);
                    break;
                
                case 'i': //push text
                    drawSpriteARGB32Scaled(epd_bitmap_push, TILE_SIZE, TILE_SIZE,
                                           TILE_SIZE, TILE_SIZE, x, y);
                    break;


                default:
                    // empty, skip
                    break;
            }
        }
    }

    // UI
    for (int x = 0; x < 800; x++) drawPixelARGB32(x, 550, 0xFFFFFFFF);
    drawString(20, 560, "     WASD: Move | B: Back | Q: Main Menu", 0xFFFFFF, 2);

    drawHUD();

    // Clear the rules area first (draw black rectangle)
    drawRectARGB32(20, 580, 800, 700, 0xFF000000, 1);
    
    // Display active rules
    drawString(20, 580, "Active Rules:", 0xFFFF00, 2);
    int ruleY = 600;
    int ruleCount = getRuleCount();
    
    for (int i = 0; i < ruleCount && i < 5; i++) {
        Rule rule = getRule(i);
        if (rule.valid) {
            char ruleText[64];
            char subjectText[16];
            char objectText[16];
            
            // Convert subject to text
            getObjectTypeText(rule.subject, subjectText);
            
            // Convert object to text  
            getObjectTypeText(rule.object, objectText);
            
            if (subjectText[0] != '\0' && objectText[0] != '\0') {
                // Manual string concatenation without sprintf
                int pos = 0;
                
                // Copy subject text
                int i = 0;
                while (subjectText[i] != '\0' && pos < 60) {
                    ruleText[pos] = subjectText[i];
                    pos++;
                    i++;
                }
                
                // Add " IS "
                ruleText[pos++] = ' ';
                ruleText[pos++] = 'I';
                ruleText[pos++] = 'S';
                ruleText[pos++] = ' ';
                
                // Copy object text
                i = 0;
                while (objectText[i] != '\0' && pos < 60) {
                    ruleText[pos] = objectText[i];
                    pos++;
                    i++;
                }
                
                ruleText[pos] = '\0';
                
                drawString(20, ruleY, ruleText, 0x00FF00, 2);
                ruleY += 20;
            }
        }
    }
}
