#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include "character.h"
#include "enemy.h"
#include "inventory.h"
#include "item.h"
#include "map.h"
#include "utilities.h"

using namespace std;

// Class representing the main game logic
class Game {
private:
    Character player;        // Player's character
    Inventory inventory;     // Player's inventory
    Map map;                 // Game map
    int currentLevel;        // Current level of the game
    int totalLevels;         // Total levels in the game
    bool gameOver;           // Is the game over?
    bool fledBattle;         // Did the player flee from battle?

    void battle(Enemy& enemy);                                      // Handles a battle with an enemy
    void showHPWarnings();                                          // Shows warnings if HP is low
    void executePlayerTurn(Enemy& enemy, bool& battleOver, bool& playerVictory, int& itemsUsed); // Handles player's turn
    void executeEnemyTurn(Enemy& enemy, bool& battleOver, bool& playerVictory);                  // Handles enemy's turn
    void performPlayerAttack(Enemy& enemy);                         // Executes player's attack
    void performEnemyAttack(Enemy& enemy);                          // Executes enemy's attack
    bool saveGame();                                                // Saves the current game state
    bool loadGame();                                                // Loads a saved game state

public:
    Game();                         // Constructor
    void start();                   // Starts the game
    void startNewGame();            // Initializes a new game
    void loadExistingGame();        // Loads an existing game
    void gameLoop();                // Main game loop
};

#endif // GAME_H_INCLUDED
