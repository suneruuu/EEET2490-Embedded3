#pragma once

void wait_msec(unsigned int n);
void set_wait_timer(int set, unsigned int msVal);
void initTimer();
void resetTimer();
int getElapsedSeconds();

