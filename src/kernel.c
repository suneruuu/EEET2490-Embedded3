#include "..\includes\uart0.h"
#include "..\includes\uart1.h"
#include "..\includes\mbox.h"
#include "..\includes\framebf.h"
#include "..\includes\color.h"


// Enums for different game states
typedef enum GameScreen { MAIN_MENU, INSTRUCTIONS, OPTIONS, CREDITS, QUIT } GameScreen;

// Function to clear the screen
void ClearScreen() {
    drawRectARGB32(0, 0, 1440, 1080, 0x00000000, 1);
}

// Function to print the main menu
void PrintMainMenu() {
    ClearScreen();
    // Centered positions and larger zoom level
    drawString(180, 100, "==================================", 0x00FFFFFF, 4);
    drawString(510, 160, "BABA IS YOU", 0x00FFFFFF, 4);
    drawString(180, 220, "==================================", 0x00FFFFFF, 4);

    // Draw button shapes and text for menu options
    drawRectARGB32(493, 336, 943, 376, 0x80808080, 1);
    drawRectARGB32(493, 336, 943, 376, 0x00000000, 0);
    drawString(510, 340, "1. Start Game", 0x00FFFFFF, 4);

    drawRectARGB32(493, 396, 1053, 436, 0x80808080, 1);
    drawRectARGB32(493, 396, 1053, 436, 0x00000000, 0);
    drawString(510, 400, "2. Instructions", 0x00FFFFFF, 4);

    drawRectARGB32(493, 456, 943, 496, 0x80808080, 1);
    drawRectARGB32(493, 456, 943, 496, 0x00000000, 0);
    drawString(510, 460, "3. Options", 0x00FFFFFF, 4);

    drawRectARGB32(493, 516, 943, 556, 0x80808080, 1);
    drawRectARGB32(493, 516, 943, 556, 0x00000000, 0);
    drawString(510, 520, "4. Credits", 0x00FFFFFF, 4);

    drawRectARGB32(493, 576, 943, 616, 0x80808080, 1);
    drawRectARGB32(493, 576, 943, 616, 0x00000000, 0);
    drawString(510, 580, "5. Quit", 0x00FFFFFF, 4);
}

// Function to print the instructions
void PrintInstructions() {
    ClearScreen();
    // Centered positions and larger zoom level
    drawString(180, 100, "==================================", 0x00FFFFFF, 4);
    drawString(510, 160, "HOW TO PLAY", 0x00FFFFFF, 4);
    drawString(180, 220, "==================================", 0x00FFFFFF, 4);

    drawString(100, 340, "The rules of the game are defined by the blocks in the level.", 0x00FFFFFF, 2);
    drawString(100, 440, "For example, if you see the blocks 'BABA IS YOU', you control Baba.", 0x00FFFFFF, 2);
    drawString(100, 540, "If the blocks say 'ROCK IS PUSH', you can push rocks.", 0x00FFFFFF, 2);
    drawString(100, 640, "When you touch a flag that is marked 'FLAG IS WIN', you win the level!", 0x00FFFFFF, 2);
    drawString(100, 740, "You can change the rules by moving the blocks around.", 0x00FFFFFF, 2);
    
    drawString(100, 840, "Press enter to return to the main menu.", 0x00FFFFFF, 4);
}

// Function to print credits
void PrintCredits() {
    ClearScreen();
    drawString(180, 100, "==================================", 0x00FFFFFF, 4);
    drawString(510, 160, "CREDITS", 0x00FFFFFF, 4);
    drawString(180, 220, "==================================", 0x00FFFFFF, 4);

    drawString(200, 400, "A game by: Arvi Teikari", 0x00FFFFFF, 4);
    drawString(200, 500, "Made with: C and QEMU", 0x00FFFFFF, 4);
    
    drawString(100, 840, "Press enter to return to the main menu.", 0x00FFFFFF, 4);
}


int main(void) {
    framebf_init();
    
    GameScreen currentScreen = MAIN_MENU;
    char choice;

    while (currentScreen != QUIT) {
        if (currentScreen == MAIN_MENU) {
            PrintMainMenu();
            
            // This is a placeholder for input. You will need a graphics-based
            // input system, not a UART-based one.
            choice = uart_getc();
            
            switch(choice) {
                case '1':
                    drawString(100, 740, "Starting game... (placeholder)", 0x00FFFFFF, 4);
                    break;
                case '2':
                    currentScreen = INSTRUCTIONS;
                    break;
                case '3':
                    drawString(100, 740, "Options... (placeholder)", 0x00FFFFFF, 4);
                    break;
                case '4':
                    currentScreen = CREDITS;
                    break;
                case '5':
                    currentScreen = QUIT;
                    break;
                default:
                    drawString(100, 740, "Invalid choice. Please try again.", 0x00FFFFFF, 4);
                    break;
            }
        } else if (currentScreen == INSTRUCTIONS) {
            PrintInstructions();
            // Wait for user input to return to the main menu
            uart_getc();
            currentScreen = MAIN_MENU;
        } else if (currentScreen == CREDITS) {
            PrintCredits();
            // Wait for user input to return to the main menu
            uart_getc();
            currentScreen = MAIN_MENU;
        }
    }
    
    drawString(100, 800, "Thank you for playing!", 0x00FFFFFF, 4);

// echo everything back
while(1) {
    //read each char
    char c = uart_getc();
    //send back
    uart_sendc(c);
    }
}

