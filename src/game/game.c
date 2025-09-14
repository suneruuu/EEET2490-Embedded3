#include "..\includes\uart0.h"
#include "..\includes\uart1.h"
#include "..\includes\mbox.h"
#include "..\includes\framebf.h"
#include "..\includes\color.h"
#include "..\assets\images\mainmenu_bg.c"
#include "..\assets\images\othermenu_bg.c"
#include "..\assets\images\levelmenu.c"
#include "map.h"
#include "player.h"

typedef enum GameScreen
{
    MAIN_MENU,
    STARTGAME,
    LEVEL,
    INSTRUCTIONS,
    OPTIONS,
    CREDITS,
    QUIT
} GameScreen;

static void ClearScreen(void);
static void PrintMainMenu(void);
static void PrintInstructions(void);
static void PrintCredits(void);
static void PrintLevel(void);
//player
#ifndef PLAYER_H
#define PLAYER_H

void initPlayer(int startX, int startY);
void drawPlayer(void);
void movePlayer(char input);

#endif
//map
#ifndef MAP_H
#define MAP_H

extern char map[24][32];
void loadLevel1(void);
void loadLevel2(void);
void drawMap(void);

#endif

// Filter CR/LF so they don't act like extra keypresses
static char read_key_filtered(void)
{
    char c;
    do
    {
        c = uart_getc();
    } while (c == '\r' || c == '\n');
    return c;
}

static void ClearScreen(void)
{
    // Opaque black (ARGB)
    drawRectARGB32(0, 0, 800, 600, 0xFF000000, 1);
}

static void PrintLevel(void)
{
    ClearScreen();
    drawImage(level, 0, 0, 800, 600);
    drawString(100, 50, "1. Level 1", 0xF54927, 3);
    drawString(450, 50, "2. Level 2", 0xF54927, 3);
    drawString(20, 560, "Press 1 or 2 to select level, or Q to return", 0xFFFFFF, 2);
}

static void PrintMainMenu(void)
{
    ClearScreen();
    drawImage(MainMenu_bgbaba_bg, 0, 0, 800, 600);
    drawString(180, 250, "1. Start Game", 0xFF80A0, 4);
    drawString(180, 310, "2. Instructions", 0xFF80A0, 4);
    drawString(180, 370, "3. Options", 0xFF80A0, 4);
    drawString(180, 430, "4. Credits", 0xFF80A0, 4);
    drawString(180, 490, "5. Quit", 0xFF80A0, 4);
}

static void PrintInstructions(void)
{
    ClearScreen();
    drawImage(othermenu_bg, 0, 0, 800, 600);
    drawString(80, 50, "====================", 0xFF80A0, 4);
    drawString(220, 100, "HOW TO PLAY", 0xFF80A0, 4);
    drawString(80, 150, "====================", 0xFF80A0, 4);

    drawString(50, 220, "1. Rules are defined by text blocks.", 0xFF80A0, 2);
    drawString(50, 260, "2. 'BABA IS YOU' means you control Baba.", 0xFF80A0, 2);
    drawString(50, 310, "3. 'ROCK IS PUSH' lets you push rocks.", 0xFF80A0, 2);
    drawString(50, 360, "4. Touch a 'FLAG IS WIN' flag to win.", 0xFF80A0, 2);
    drawString(50, 410, "5. Move text blocks to change rules.", 0xFF80A0, 2);

    drawString(50, 560, "Press any key to return to the main menu.", 0xFF80A0, 2);
}

static void PrintCredits(void)
{
    ClearScreen();
    drawImage(othermenu_bg, 0, 0, 800, 600);
    drawString(80, 50, "====================", 0xFF80A0, 4);
    drawString(220, 100, "CREDITS", 0xFF80A0, 4);
    drawString(80, 150, "====================", 0xFF80A0, 4);

    drawString(50, 220, "A game by: Arvi Teikari", 0xFF80A0, 2);
    drawString(50, 250, "Made with: C and QEMU", 0xFF80A0, 2);

    drawString(50, 560, "Press any key to return to the main menu.", 0xFF80A0, 2);
}

int main(void)
{
    framebf_init();

    GameScreen currentScreen = MAIN_MENU;
    int currentLevel = 0;

    while (currentScreen != QUIT)
    {

        if (currentScreen == MAIN_MENU)
        {
            PrintMainMenu();
            for (;;)
            {
                char choice = read_key_filtered();
                if (choice == '1')
                {
                    currentScreen = STARTGAME;
                    break;
                }
                if (choice == '2')
                {
                    currentScreen = INSTRUCTIONS;
                    break;
                }
                if (choice == '3')
                {
                    drawString(50, 520, "Options... (placeholder)", 0xFFFFFF, 3);
                    continue;
                }
                if (choice == '4')
                {
                    currentScreen = CREDITS;
                    break;
                }
                if (choice == '5')
                {
                    currentScreen = QUIT;
                    break;
                }
                // else ignore invalid keys and keep waiting
            }
        }

        else if (currentScreen == INSTRUCTIONS)
        {
            PrintInstructions();
            (void)read_key_filtered();
            currentScreen = MAIN_MENU;
        }

        else if (currentScreen == CREDITS)
        {
            PrintCredits();
            (void)read_key_filtered();
            currentScreen = MAIN_MENU;
        }

        else if (currentScreen == STARTGAME)
        {
            PrintLevel();
            for (;;)
            {
                char level_choice = read_key_filtered();
                if (level_choice == '1')
                {
                    currentLevel = 1;
                    currentScreen = LEVEL;
                    break;
                }
                else if (level_choice == '2')
                {
                    currentLevel = 2;
                    currentScreen = LEVEL;
                    break;
                }
                else if (level_choice == 'q' || level_choice == 'Q')
                {
                    currentScreen = MAIN_MENU;
                    break;
                }
                // else: ignore and keep waiting
            }
        }

        else if (currentScreen == LEVEL)
        {
            // Draw once to avoid flicker
            ClearScreen();
            switch (currentLevel)
            {
            case 1:
                LoadLevel1();
                break;
            case 2:
                LoadLevel2();
                break;
            default:
                break;
            }
            drawMap();
            initPlayer(13,7);
            // Inner loop: wait for commands; only redraw on changes
            for (;;)
            {
                char c = read_key_filtered();
                if (c == 'Q' || c == 'q')
                { // back to main
                    currentScreen = MAIN_MENU;
                    break;
                }
                else if (c == 'B' || c == 'b')
                { // back to level select
                    currentScreen = STARTGAME;
                    break;
                }
                else
                {
                    movePlayer(c);
                }
            }
        }
    }

    // Farewell screen
    ClearScreen();
    drawString(300, 300, "Goodbye!", 0xFFFFFF, 4);

    // Echo loop
    while (1)
    {
        char c = uart_getc();
        uart_sendc(c);
    }
}
