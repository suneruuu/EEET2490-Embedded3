// ----------------------------------- framebf.h -------------------------------------
#pragma once
void framebf_init();
void drawPixelARGB32(int x, int y, unsigned int attr);
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill);
void drawLineARGB32(int y, int x1, int x2, unsigned int attr);
void drawCircleARGB32(int x1, int y1, int r, unsigned int attr, int fill);
int check(int px, int py, int x, int y, int r);
