#include "../../includes/game/game_logic.h"
#include "../../includes/game/player.h"

// Global game state
static GameState gameState;

// Initialize game logic
void initGameLogic(void) {
    gameState.playerX = 0;
    gameState.playerY = 0;
    gameState.gameWon = 0;
    gameState.rulesChanged = 0;
    gameState.ruleCount = 0;
    // Clear rules array manually
    for (int i = 0; i < 10; i++) {
        gameState.rules[i].subject = OBJECT_NONE;
        gameState.rules[i].verb = OBJECT_NONE;
        gameState.rules[i].object = OBJECT_NONE;
        gameState.rules[i].valid = 0;
    }
}

// Reset game state
void resetGameState(void) {
    gameState.gameWon = 0;
    gameState.rulesChanged = 0;
    gameState.ruleCount = 0;
    // Clear rules array manually
    for (int i = 0; i < 10; i++) {
        gameState.rules[i].subject = OBJECT_NONE;
        gameState.rules[i].verb = OBJECT_NONE;
        gameState.rules[i].object = OBJECT_NONE;
        gameState.rules[i].valid = 0;
    }
}

// Get player position
void getPlayerPosition(int *x, int *y) {
    *x = gameState.playerX;
    *y = gameState.playerY;
}

// Set player position
void setPlayerPosition(int x, int y) {
    gameState.playerX = x;
    gameState.playerY = y;
}

// Check if character is a text block (lowercase)
int isTextBlock(char c) {
    return (c >= 'a' && c <= 'z');
}

// Check if character is an image block (uppercase)
int isImageBlock(char c) {
    return (c >= 'A' && c <= 'Z');
}

// Convert character to object type
ObjectType charToObjectType(char c) {
    switch (c) {
        case 'a': return OBJECT_BABA;
        case 'b': return OBJECT_IS;
        case 'c': return OBJECT_YOU;
        case 'd': return OBJECT_FLAG;
        case 'e': return OBJECT_WIN;
        case 'f': return OBJECT_STOP;
        case 'g': return OBJECT_WALL;
        case 'h': return OBJECT_ROCK;
        case 'i': return OBJECT_PUSH;
        // case 'j': return OBJECT_DEFEAT;  // commented out for now
        // case 'k': return OBJECT_SINK;    // commented out for now
        case 'R': return OBJECT_ROCK_IMG;
        case 'W': return OBJECT_WALL_IMG;
        case 'E': return OBJECT_WALL_IMG;
        case 'Q': return OBJECT_WALL_IMG;
        case 'Z': return OBJECT_FLAG;
        default: return OBJECT_NONE;
    }
}

// Convert object type to character
char objectTypeToChar(ObjectType type) {
    switch (type) {
        case OBJECT_BABA: return 'a';
        case OBJECT_IS: return 'b';
        case OBJECT_YOU: return 'c';
        case OBJECT_FLAG: return 'd';
        case OBJECT_WIN: return 'e';
        case OBJECT_STOP: return 'f';
        case OBJECT_WALL: return 'g';
        case OBJECT_ROCK: return 'h';
        case OBJECT_PUSH: return 'i';
        // case OBJECT_DEFEAT: return 'j';  // commented out for now
        // case OBJECT_SINK: return 'k';    // commented out for now
        case OBJECT_ROCK_IMG: return 'R';
        case OBJECT_WALL_IMG: return 'W';
        default: return ' ';
    }
}

// Check if an object type is a noun (can be subject of rules)
int isNoun(ObjectType type) {
    return (type == OBJECT_BABA || type == OBJECT_FLAG || 
            type == OBJECT_WALL || type == OBJECT_WALL_IMG || 
            type == OBJECT_ROCK || type == OBJECT_ROCK_IMG);
}

// Check if an object type is an attribute (can be object of rules)
int isAttribute(ObjectType type) {
    return (type == OBJECT_YOU || type == OBJECT_WIN || type == OBJECT_STOP || 
            type == OBJECT_PUSH); // || type == OBJECT_DEFEAT || type == OBJECT_SINK);  // commented out for now
}

// Check if a rule already exists
int ruleExists(Rule newRule) {
    for (int i = 0; i < gameState.ruleCount; i++) {
        if (gameState.rules[i].valid && 
            gameState.rules[i].subject == newRule.subject &&
            gameState.rules[i].verb == newRule.verb &&
            gameState.rules[i].object == newRule.object) {
            return 1; // Rule already exists
        }
    }
    return 0;
}

// Add a rule if it doesn't already exist
void addRuleIfNew(Rule rule) {
    if (!ruleExists(rule) && gameState.ruleCount < 10) {
        gameState.rules[gameState.ruleCount] = rule;
        gameState.ruleCount++;
    }
}

// Parse rules from the map
void parseRules(void) {
    gameState.ruleCount = 0;
    
    // Look for horizontal rules (left to right) - adjacent text blocks
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH - 2; x++) {
            char c1 = map[y][x];
            char c2 = map[y][x + 1];
            char c3 = map[y][x + 2];
            
            // Check for "abc" pattern (subject IS object) - adjacent text blocks
            if (isTextBlock(c1) && isTextBlock(c2) && isTextBlock(c3)) {
                ObjectType subject = charToObjectType(c1);
                ObjectType verb = charToObjectType(c2);
                ObjectType object = charToObjectType(c3);
                
                // Check if it's a valid rule pattern: Noun IS (Noun|Attribute)
                if (isNoun(subject) && verb == OBJECT_IS && 
                    (isNoun(object) || isAttribute(object))) {
                    // Found a rule: subject IS object
                    Rule rule;
                    rule.subject = subject;
                    rule.verb = verb;
                    rule.object = object;
                    rule.valid = 1;
                    
                    addRuleIfNew(rule);
                }
            }
        }
    }
    
    // Look for vertical rules (top to bottom) - adjacent text blocks
    for (int y = 0; y < MAP_HEIGHT - 2; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            char c1 = map[y][x];
            char c2 = map[y + 1][x];
            char c3 = map[y + 2][x];
            
            // Check for vertical "abc" pattern (subject IS object) - adjacent text blocks
            if (isTextBlock(c1) && isTextBlock(c2) && isTextBlock(c3)) {
                ObjectType subject = charToObjectType(c1);
                ObjectType verb = charToObjectType(c2);
                ObjectType object = charToObjectType(c3);
                
                // Check if it's a valid rule pattern: Noun IS (Noun|Attribute)
                if (isNoun(subject) && verb == OBJECT_IS && 
                    (isNoun(object) || isAttribute(object))) {
                    // Found a rule: subject IS object
                    Rule rule;
                    rule.subject = subject;
                    rule.verb = verb;
                    rule.object = object;
                    rule.valid = 1;
                    
                    addRuleIfNew(rule);
                }
            }
        }
    }
    
    // Look for horizontal rules (right to left) - adjacent text blocks
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = MAP_WIDTH - 1; x >= 2; x--) {
            char c1 = map[y][x];
            char c2 = map[y][x - 1];
            char c3 = map[y][x - 2];
            
            // Check for "cba" pattern (object IS subject) - adjacent text blocks
            if (isTextBlock(c1) && isTextBlock(c2) && isTextBlock(c3)) {
                ObjectType object = charToObjectType(c1);
                ObjectType verb = charToObjectType(c2);
                ObjectType subject = charToObjectType(c3);
                
                // Check if it's a valid rule pattern: Noun IS (Noun|Attribute)
                if (isNoun(subject) && verb == OBJECT_IS && 
                    (isNoun(object) || isAttribute(object))) {
                    // Found a rule: subject IS object
                    Rule rule;
                    rule.subject = subject;
                    rule.verb = verb;
                    rule.object = object;
                    rule.valid = 1;
                    
                    addRuleIfNew(rule);
                }
            }
        }
    }
    
    // Look for vertical rules (bottom to top) - adjacent text blocks
    for (int y = MAP_HEIGHT - 1; y >= 2; y--) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            char c1 = map[y][x];
            char c2 = map[y - 1][x];
            char c3 = map[y - 2][x];
            
            // Check for vertical "abc" pattern (subject IS object) - adjacent text blocks
            if (isTextBlock(c1) && isTextBlock(c2) && isTextBlock(c3)) {
                ObjectType subject = charToObjectType(c1);
                ObjectType verb = charToObjectType(c2);
                ObjectType object = charToObjectType(c3);
                
                // Check if it's a valid rule pattern: Noun IS (Noun|Attribute)
                if (isNoun(subject) && verb == OBJECT_IS && 
                    (isNoun(object) || isAttribute(object))) {
                    // Found a rule: subject IS object
                    Rule rule;
                    rule.subject = subject;
                    rule.verb = verb;
                    rule.object = object;
                    rule.valid = 1;
                    
                    addRuleIfNew(rule);
                }
            }
        }
    }
}

// Check if player is controlled by rules
int isPlayerControlled(void) {
    // If no rules exist, player can move freely (floating objects)
    if (gameState.ruleCount == 0) {
        return 1;
    }
    
    // Check if any object has YOU property
    return hasAnyYouObject();
}

// Check if any object has YOU property
int hasAnyYouObject(void) {
    for (int i = 0; i < gameState.ruleCount; i++) {
        if (gameState.rules[i].valid && gameState.rules[i].object == OBJECT_YOU) {
            return 1;
        }
    }
    return 0;
}

// Check if a specific object type has YOU property
int isObjectTypeYou(ObjectType objectType) {
    for (int i = 0; i < gameState.ruleCount; i++) {
        if (gameState.rules[i].valid && 
            gameState.rules[i].subject == objectType && 
            gameState.rules[i].object == OBJECT_YOU) {
            return 1;
        }
    }
    
    // Special case: if checking for WALL_IMG, also check if WALL has YOU property
    if (objectType == OBJECT_WALL_IMG) {
        for (int i = 0; i < gameState.ruleCount; i++) {
            if (gameState.rules[i].valid && 
                gameState.rules[i].subject == OBJECT_WALL && 
                gameState.rules[i].object == OBJECT_YOU) {
                return 1;
            }
        }
    }
    
    // Special case: if checking for ROCK_IMG, also check if ROCK has YOU property
    if (objectType == OBJECT_ROCK_IMG) {
        for (int i = 0; i < gameState.ruleCount; i++) {
            if (gameState.rules[i].valid && 
                gameState.rules[i].subject == OBJECT_ROCK && 
                gameState.rules[i].object == OBJECT_YOU) {
                return 1;
            }
        }
    }
    
    return 0;
}


// Convert object type to text string
void getObjectTypeText(ObjectType objectType, char* text) {
    const char* source = "";
    
    switch (objectType) {
        case OBJECT_BABA: source = "BABA"; break;
        case OBJECT_ROCK: source = "ROCK"; break;
        case OBJECT_WALL: source = "WALL"; break;
        case OBJECT_FLAG: source = "FLAG"; break;
        case OBJECT_IS: source = "IS"; break;
        case OBJECT_YOU: source = "YOU"; break;
        case OBJECT_WIN: source = "WIN"; break;
        case OBJECT_PUSH: source = "PUSH"; break;
        case OBJECT_STOP: source = "STOP"; break;
        case OBJECT_ROCK_IMG: source = "ROCK"; break;
        case OBJECT_WALL_IMG: source = "WALL"; break;
        default: source = ""; break;
    }
    
    // Manual string copy without strcpy
    int i = 0;
    while (source[i] != '\0' && i < 15) {
        text[i] = source[i];
        i++;
    }
    text[i] = '\0';
}

// Check if something is pushable
int isPushable(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return 0;
    
    char c = map[y][x];
    
    // Text blocks are always pushable
    if (isTextBlock(c)) return 1;
    
    // Check if image block has PUSH property
    ObjectType objectType = charToObjectType(c);
    return isPush(objectType);
}

// Check if something is stopped
int isStopped(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return 0;
    
    char c = map[y][x];
    ObjectType objectType = charToObjectType(c);
    
    // Check if object has STOP property
    return isStop(objectType);
}

// Check if player can move to position
int canMoveTo(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return 0;
    if (y >= 21) return 0; // UI region
    
    char c = map[y][x];
    
    // Empty space and floor are always passable
    if (c == ' ' || c == 'F') return 1;
    
    // If no rules exist, all objects are passable (floating)
    if (gameState.ruleCount == 0) {
        return 1;
    }
    
    // Check for DEFEAT collision - player can move but will be defeated
    // if (checkDefeatCollision(x, y)) {  // commented out for now
    //     return 1; // Allow movement, collision will be handled
    // }
    
    // Check for SINK collision - player can move but will sink
    // if (checkSinkCollision(x, y)) {    // commented out for now
    //     return 1; // Allow movement, collision will be handled
    // }
    
    // Check if position is stopped by rules
    if (isStopped(x, y)) return 0;
    
    // Text blocks can be pushed (but not stepped on directly)
    if (isTextBlock(c)) {
        return 0; // Can't step on text blocks, but can push them
    }
    
    // Image blocks - check if pushable by rules
    if (isImageBlock(c) && isPushable(x, y)) {
        return 0; // Can't step on pushable objects, but can push them
    }
    
    // If no rules apply, treat as passable (floating)
    return 1;
}

// Check if player can move to position based on current YOU object
int canMoveToAsYou(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return 0;
    if (y >= 21) return 0; // UI region
    
    char c = map[y][x];
    
    // Empty space and floor are always passable
    if (c == ' ' || c == 'F') return 1;
    
    // If no rules exist, all objects are passable (floating)
    if (gameState.ruleCount == 0) {
        return 1;
    }
    
    // Check if position is stopped by rules FIRST
    if (isStopped(x, y)) return 0;
    
    // Text blocks can be pushed (but not stepped on directly)
    if (isTextBlock(c)) {
        return 0; // Can't step on text blocks, but can push them
    }
    
    // Check if the object at this position has YOU property
    ObjectType objectType = charToObjectType(c);
    if (isObjectTypeYou(objectType)) {
        return 1; // Can move to objects that have YOU property
    }
    
    // Image blocks - check if pushable by rules
    if (isImageBlock(c) && isPushable(x, y)) {
        return 0; // Can't step on pushable objects, but can push them
    }
    
    // If no rules apply, treat as passable (floating)
    return 1;
}

// Check if text block can be pushed (with chain pushing support)
int canPushTextBlock(int fromX, int fromY, int toX, int toY) {
    if (toX < 0 || toX >= MAP_WIDTH || toY < 0 || toY >= MAP_HEIGHT) return 0;
    if (toY >= 21) return 0; // UI region
    
    char c = map[toY][toX];
    
    // Can push to empty space or floor
    if (c == ' ' || c == 'F') return 1;
    
    // Can push to another text block if we can push that text block further
    if (isTextBlock(c)) {
        int chainPushX = toX + (toX - fromX);
        int chainPushY = toY + (toY - fromY);
        return canPushTextBlock(toX, toY, chainPushX, chainPushY);
    }
    
    // Can push through image blocks that don't have STOP property
    if (isImageBlock(c)) {
        ObjectType objectType = charToObjectType(c);
        if (!isStop(objectType)) {
            return 1; // Can push through non-stop image blocks
        }
    }
    
    return 0;
}

// Push text block
void pushTextBlock(int fromX, int fromY, int toX, int toY) {
    if (fromX < 0 || fromX >= MAP_WIDTH || fromY < 0 || fromY >= MAP_HEIGHT) return;
    if (toX < 0 || toX >= MAP_WIDTH || toY < 0 || toY >= MAP_HEIGHT) return;
    if (toY >= 21) return; // UI region
    
    char c = map[fromY][fromX];
    map[toY][toX] = c;
    map[fromY][fromX] = ' '; // Replace text block with empty space
    
    // Mark that rules might have changed
    gameState.rulesChanged = 1;
}

// Push any pushable object with chain pushing
void pushObject(int fromX, int fromY, int toX, int toY) {
    if (fromX < 0 || fromX >= MAP_WIDTH || fromY < 0 || fromY >= MAP_HEIGHT) return;
    if (toX < 0 || toX >= MAP_WIDTH || toY < 0 || toY >= MAP_HEIGHT) return;
    if (toY >= 21) return; // UI region
    
    char c = map[fromY][fromX];
    char destinationChar = map[toY][toX];
    
    // If destination has a text block, we need to handle chain pushing
    if (isTextBlock(destinationChar)) {
        // Calculate where to push the existing text block
        int chainPushX = toX + (toX - fromX);
        int chainPushY = toY + (toY - fromY);
        
        // Check if we can push the chain
        if (!canPushTextBlock(toX, toY, chainPushX, chainPushY)) {
            return; // Can't push the chain, so abort
        }
        
        // Recursively push the destination text block first
        pushObject(toX, toY, chainPushX, chainPushY);
    }
    // If destination has a pushable image block, push it first
    else if (isImageBlock(destinationChar) && isPushable(toX, toY)) {
        // Calculate where to push the existing object
        int chainPushX = toX + (toX - fromX);
        int chainPushY = toY + (toY - fromY);
        
        // Check if we can push the object
        if (chainPushX < 0 || chainPushX >= MAP_WIDTH || chainPushY < 0 || chainPushY >= MAP_HEIGHT) {
            return; // Can't push out of bounds
        }
        if (chainPushY >= 21) return; // UI region
        
        // Check if destination is clear or pushable
        char nextChar = map[chainPushY][chainPushX];
        if (nextChar != ' ' && nextChar != 'F' && !isTextBlock(nextChar) && !(isImageBlock(nextChar) && isPushable(chainPushX, chainPushY))) {
            return; // Can't push to occupied space
        }
        
        // Recursively push the destination object first
        pushObject(toX, toY, chainPushX, chainPushY);
    }
    // If destination has a non-stop image block, we can push through it
    else if (isImageBlock(destinationChar)) {
        ObjectType objectType = charToObjectType(destinationChar);
        if (!isStop(objectType)) {
            // Can push through non-stop image blocks - just replace them
            // Move the text block to the destination
            map[toY][toX] = map[fromY][fromX];
            map[fromY][fromX] = ' '; // Replace with empty space
        } else {
            return; // Can't push through stop objects
        }
    }
    // If destination is not empty and not pushable, we can't move there
    else if (destinationChar != ' ' && destinationChar != 'F') {
        return; // Can't move to occupied space
    }
    
    // Now move the original object
    map[toY][toX] = c;
    
    // Replace with empty space for all objects
    map[fromY][fromX] = ' '; // Replace with empty space
    
    // Mark that rules might have changed
    gameState.rulesChanged = 1;
}

// Check if an object has a specific property through rules
int hasProperty(ObjectType objectType, ObjectType property) {
    for (int i = 0; i < gameState.ruleCount; i++) {
        if (gameState.rules[i].valid && 
            gameState.rules[i].subject == objectType && 
            gameState.rules[i].object == property) {
            return 1;
        }
    }
    
    // Special case: ROCK_IMG should also check ROCK properties
    if (objectType == OBJECT_ROCK_IMG) {
        for (int i = 0; i < gameState.ruleCount; i++) {
            if (gameState.rules[i].valid && 
                gameState.rules[i].subject == OBJECT_ROCK && 
                gameState.rules[i].object == property) {
                return 1;
            }
        }
    }
    
    // Special case: WALL_IMG should also check WALL properties
    if (objectType == OBJECT_WALL_IMG) {
        for (int i = 0; i < gameState.ruleCount; i++) {
            if (gameState.rules[i].valid && 
                gameState.rules[i].subject == OBJECT_WALL && 
                gameState.rules[i].object == property) {
                return 1;
            }
        }
    }
    
    return 0;
}

// Property checking functions
int isYou(ObjectType objectType) {
    return hasProperty(objectType, OBJECT_YOU);
}

int isWin(ObjectType objectType) {
    return hasProperty(objectType, OBJECT_WIN);
}

int isPush(ObjectType objectType) {
    return hasProperty(objectType, OBJECT_PUSH);
}

int isStop(ObjectType objectType) {
    return hasProperty(objectType, OBJECT_STOP);
}

// int isDefeat(ObjectType objectType) {  // commented out for now
//     return hasProperty(objectType, OBJECT_DEFEAT);
// }

// int isSink(ObjectType objectType) {    // commented out for now
//     return hasProperty(objectType, OBJECT_SINK);
// }

// Check if player would be defeated by moving to position
// int checkDefeatCollision(int x, int y) {  // commented out for now
//     if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return 0;
//     
//     char c = map[y][x];
//     ObjectType objectType = charToObjectType(c);
//     
//     // Check if object has DEFEAT property
//     return isDefeat(objectType);
// }

// Check if player would sink by moving to position
// int checkSinkCollision(int x, int y) {    // commented out for now
//     if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return 0;
//     
//     char c = map[y][x];
//     ObjectType objectType = charToObjectType(c);
//     
//     // Check if object has SINK property
//     return isSink(objectType);
// }

// Handle collision at position (for DEFEAT and SINK properties)
void handleCollision(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return;
    
    // char c = map[y][x];
    // ObjectType objectType = charToObjectType(c);
    
    // Handle DEFEAT collision - player is destroyed
    // if (isDefeat(objectType)) {  // commented out for now
    //     // For now, just reset the game state
    //     // In a full implementation, you might want to restart the level
    //     resetGameState();
    //     return;
    // }
    
    // Handle SINK collision - both objects are destroyed
    // if (isSink(objectType)) {    // commented out for now
    //     // Remove the sink object
    //     map[y][x] = ' ';
    //     // Player is also destroyed
    //     resetGameState();
    //     return;
    // }
}

// Check win condition
int isWinCondition(void) {
    // Check if player is touching an object that has WIN property
    char c = map[gameState.playerY][gameState.playerX];
    ObjectType objectType = charToObjectType(c);
    
    // Check if the object at player position has WIN property
    if (isWin(objectType)) {
        return 1; // Player touches WIN object
    }
    
    // Check if the object that has YOU property also has WIN property
    // This handles cases like "ROCK IS YOU" + "ROCK IS WIN"
    if (isYou(OBJECT_BABA) && isWin(OBJECT_BABA)) {
        return 1; // BABA IS YOU and BABA IS WIN
    }
    if (isYou(OBJECT_FLAG) && isWin(OBJECT_FLAG)) {
        return 1; // FLAG IS YOU and FLAG IS WIN
    }
    if (isYou(OBJECT_WALL) && isWin(OBJECT_WALL)) {
        return 1; // WALL IS YOU and WALL IS WIN
    }
    if (isYou(OBJECT_ROCK) && isWin(OBJECT_ROCK)) {
        return 1; // ROCK IS YOU and ROCK IS WIN
    }
    if (isYou(OBJECT_ROCK_IMG) && isWin(OBJECT_ROCK_IMG)) {
        return 1; // ROCK_IMG IS YOU and ROCK_IMG IS WIN
    }
    
    return 0;
}

// Check if game is won
int isGameWon(void) {
    return gameState.gameWon;
}

// Check if game is lost (no YOU rule active)
int isGameLost(void) {
    // If there are rules but no YOU rule, player loses
    if (gameState.ruleCount > 0 && !hasAnyYouObject()) {
        return 1;
    }
    
    // Game continues as long as any object has YOU property
    return 0;
}

// Update game logic
void updateGameLogic(void) {
    // Parse rules if they changed
    if (gameState.rulesChanged) {
        parseRules();
        gameState.rulesChanged = 0;
    }
    
    // Check win condition
    if (isWinCondition()) {
        gameState.gameWon = 1;
    }
    
    // Check lose condition (no YOU rule active)
    if (isGameLost()) {
        gameState.gameWon = -1; // Use -1 to indicate game over
    }
}

// Get rule count
int getRuleCount(void) {
    return gameState.ruleCount;
}

// Get rule at index
Rule getRule(int index) {
    if (index >= 0 && index < gameState.ruleCount) {
        return gameState.rules[index];
    }
    Rule emptyRule = {OBJECT_NONE, OBJECT_NONE, OBJECT_NONE, 0};
    return emptyRule;
}

