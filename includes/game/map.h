#ifndef MAP_H
#define MAP_H

#include "../../includes/framebf.h"

#define MAP_WIDTH 32
#define MAP_HEIGHT 24
#define TILE_SIZE 25

extern char map[MAP_HEIGHT][MAP_WIDTH];  // declaration only

// Level loading
void LoadLevel1(void);
void LoadLevel2(void);

// Drawing
void drawMap(void);

#endif
