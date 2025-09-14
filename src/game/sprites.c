#include "..\includes\sprites.h"
#include "..\assets\images\spritesheet.c"

// Define frames
const Frame BABA_IDLE = {0, 0};
const Frame BABA_WALK_RIGHT[2] = {{24,0}, {48,0}};
const Frame BABA_WALK_LEFT[2]  = {{0,24}, {24,24}};
const Frame BABA_WALK_UP[2]    = {{0,48}, {24,48}};
const Frame BABA_WALK_DOWN[2]  = {{0,72}, {24,72}};

extern const unsigned long spritesheet[]; 
extern const unsigned long* spritesheetallArray[];
extern const int spritesheetallArray_LEN;

void drawSpriteTile(int srcX, int srcY, int w, int h,
                    int dstX, int dstY, int sheetWidth)
{
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            unsigned long color =
                spritesheet[(srcY + y) * sheetWidth + (srcX + x)];

            // only draw non-transparent
            if ((color >> 24) != 0x00) {
                drawPixelARGB32(dstX + x, dstY + y, color);
            }
        }
    }
}


