#include "..\includes\uart0.h"
#include "..\includes\uart1.h"
#include "..\includes\mbox.h"
#include "..\includes\framebf.h"
#include "..\includes\color.h"
#include "..\assets\mainmenu_bg.c"
#include "..\assets\images\othermenu_bg.c"
// Enums for different game states
typedef enum GameScreen { MAIN_MENU, INSTRUCTIONS, OPTIONS, CREDITS, QUIT } GameScreen;

// Function to clear the screen
void ClearScreen() {
    drawRectARGB32(0, 0, 800, 600, 0x00000000, 1);
}

// Function to print the main menu
void PrintMainMenu() {
    ClearScreen();
    //Background for main menu
    drawImage(MainMenu_bgbaba_bg, 0, 0, 800, 600);
    // Draw text for menu options
    drawString(180, 250, "1. Start Game", 0xFF80A0, 4);
    drawString(180, 310, "2. Instructions", 0xFF80A0, 4);
    drawString(180, 370, "3. Options", 0xFF80A0, 4);
    drawString(180, 430, "4. Credits", 0xFF80A0, 4);
    drawString(180, 490, "5. Quit", 0xFF80A0, 4);
}

// Function to print the instructions
void PrintInstructions() {
    ClearScreen();
    //background
    drawImage(othermenu_bg, 0, 0, 800, 600);
    // Centered positions and larger zoom level for 800x600 screen
    drawString(80, 50, "====================", 0xFF80A0, 4);
    drawString(220, 100, "HOW TO PLAY", 0xFF80A0, 4);
    drawString(80, 150, "====================", 0xFF80A0, 4);

    drawString(50, 220, "1. The rules of the game are defined by the", 0xFF80A0, 2);
    drawString(50, 260, "blocks in the level.", 0xFF80A0, 2);
    
    drawString(50, 310, "2. For example, if you see the blocks 'BABA", 0xFF80A0, 2);
    drawString(50, 330, "IS YOU', you control Baba.", 0xFF80A0, 2);
    
    drawString(50, 380, "3. If the blocks say 'ROCK IS PUSH', you can", 0xFF80A0, 2);
    drawString(50, 400, "push rocks.", 0xFF80A0, 2);
    
    drawString(50, 450, "4. When you touch a flag that is marked", 0xFF80A0, 2);
    drawString(50, 470, "'FLAG IS WIN', you win the level!", 0xFF80A0, 2);
    
    drawString(50, 520, "5. You can change the rules by moving the", 0xFF80A0, 2);
    drawString(50, 540, "blocks around.", 0xFF80A0, 2);
    
    drawString(50, 570, "Press ENTER to return to the main menu.", 0xFF80A0, 2);
}

// Function to print credits
void PrintCredits() {
    ClearScreen();
    drawImage(othermenu_bg, 0, 0, 800, 600);
    drawString(80, 50, "====================", 0xFF80A0, 4);
    drawString(220, 100, "CREDITS", 0xFF80A0, 4);
    drawString(80, 150, "====================", 0xFF80A0, 4);
    
    drawString(50, 220, "A game by: Arvi Teikari", 0xFF80A0, 2);
    
    drawString(50, 250, "Made with: C and QEMU", 0xFF80A0, 2);

    drawString(50, 520, "Press ENTER to return to the main menu.", 0xFF80A0, 2);
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
            
            // The following code is for demonstration and would need to be replaced by your input handling logic
            // We will use a placeholder '2' to demonstrate the instructions screen.
            
            switch(choice) {
                case '1':
                    drawString(50, 500, "Starting game... (placeholder)", 0x00FFFFFF, 4);
                    break;
                case '2':
                    currentScreen = INSTRUCTIONS;
                    break;
                case '3':
                    drawString(50, 500, "Options... (placeholder)", 0x00FFFFFF, 4);
                    break;
                case '4':
                    currentScreen = CREDITS;
                    break;
                case '5':
                    currentScreen = QUIT;
                    break;
                default:
                    drawString(50, 500, "Invalid choice. Please try again.", 0x00FFFFFF, 4);
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
    
    ClearScreen();
    drawString(300, 300, "Goodbye!", 0x00FFFFFF, 4);

    // echo everything back
    while(1) {
        //read each char
        char c = uart_getc();
        //send back
        uart_sendc(c);
    }
}
