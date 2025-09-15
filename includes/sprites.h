#ifndef SPRITES_H
#define SPRITES_H

#include "framebf.h"

#define TILE_SIZE 25  

typedef struct {
    const unsigned long* data;
    int width;
    int height;
} Frame;

// Sprites
extern const unsigned long sprite00[];
extern const unsigned long sprite01[];
extern const unsigned long sprite02[];
extern const unsigned long sprite03[];
extern const unsigned long sprite04[];
extern const unsigned long sprite05[];
extern const unsigned long sprite06[];
extern const unsigned long sprite07[];

// Animations
extern const Frame BABA_IDLE;
extern const Frame BABA_WALK_RIGHT[2];
extern const Frame BABA_WALK_DOWN[2];
extern const Frame BABA_WALK_LEFT[2];
extern const Frame BABA_WALK_UP[2];

void drawSpriteARGB32(const unsigned long* sprite, int w, int h,
                      int posX, int posY);

#endif
