#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "map.h"

// Game object types
typedef enum {
    OBJECT_NONE = 0,
    // Nouns (objects)
    OBJECT_BABA,    // 'a' text
    OBJECT_FLAG,    // 'd' text, 'Z' image
    OBJECT_WALL,    // 'g' text
    OBJECT_WALL_IMG, // 'W' image
    OBJECT_ROCK,    // 'h' text
    OBJECT_ROCK_IMG, // 'R' image
    // Words
    OBJECT_IS,      // 'b' text
    OBJECT_YOU,     // 'c' text
    OBJECT_WIN,     // 'e' text
    OBJECT_STOP,    // 'f' text
    OBJECT_PUSH,    // 'i' text
    // OBJECT_DEFEAT,  // 'j' text - commented out for now
    // OBJECT_SINK     // 'k' text - commented out for now
} ObjectType;

// Rule structure
typedef struct {
    ObjectType subject;
    ObjectType verb;
    ObjectType object;
    int valid; // 1 if rule is valid, 0 if not
} Rule;

// Game state
typedef struct {
    int playerX, playerY;
    int gameWon;
    int rulesChanged;
    Rule rules[10]; // Maximum 10 rules
    int ruleCount;
} GameState;

// Function declarations
void initGameLogic(void);
void updateGameLogic(void);
int canMoveTo(int x, int y);
int canMoveToAsYou(int x, int y);
void getObjectTypeText(ObjectType objectType, char* text);
int canPushTextBlock(int fromX, int fromY, int toX, int toY);
void pushTextBlock(int fromX, int fromY, int toX, int toY);
void pushObject(int fromX, int fromY, int toX, int toY);
void parseRules(void);
void applyRules(void);
int isWinCondition(void);
int isPlayerControlled(void);
int isPushable(int x, int y);
int isStopped(int x, int y);
void getPlayerPosition(int *x, int *y);
void setPlayerPosition(int x, int y);
int isGameWon(void);
int isGameLost(void);
void resetGameState(void);

// Helper functions
ObjectType charToObjectType(char c);
char objectTypeToChar(ObjectType type);
int isTextBlock(char c);
int isImageBlock(char c);
int isNoun(ObjectType type);
int isAttribute(ObjectType type);

// Rule access functions
int getRuleCount(void);
Rule getRule(int index);

// Property checking functions
int hasProperty(ObjectType objectType, ObjectType property);
int isYou(ObjectType objectType);
int isWin(ObjectType objectType);
int hasAnyYouObject(void);
int isPush(ObjectType objectType);
int isStop(ObjectType objectType);
// int isDefeat(ObjectType objectType);  // commented out for now
// int isSink(ObjectType objectType);    // commented out for now

// Collision handling functions
void handleCollision(int x, int y);
// int checkDefeatCollision(int x, int y);  // commented out for now
// int checkSinkCollision(int x, int y);    // commented out for now

#endif
