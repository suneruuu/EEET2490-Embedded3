#ifndef SPRITES_H
#define SPRITES_H

#include "framebf.h"

#define TILE_SIZE 32
#define SHEET_WIDTH 1156  

typedef struct {
    int x;
    int y;
} Frame;

extern const Frame BABA_WALK_RIGHT[2];
extern const Frame BABA_WALK_LEFT[2];
extern const Frame BABA_WALK_UP[2];
extern const Frame BABA_WALK_DOWN[2];
// Spritesheet
extern const unsigned long spritesheet[];
extern const unsigned long* spritesheetallArray[];
extern const int spritesheetallArray_LEN;

void drawSpriteTile(int srcX, int srcY, int tileW, int tileH,
                    int destX, int destY, int sheetW);

#endif
