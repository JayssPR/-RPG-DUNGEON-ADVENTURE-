#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <string>
#include <vector>
#include "character.h"
#include "inventory.h"
#include "enemy.h"
#include "item.h"
#include "utilities.h"
using namespace std;

// Structure representing a single tile on the map
struct Tile {
    string symbol;     // Visual symbol for the tile
    bool hasEnemy;     // Does the tile contain an enemy?
    bool hasItem;      // Does the tile contain an item?
    Item item;         // Item present on the tile
    Enemy* enemy;      // Pointer to the enemy (if any)
};

// Class representing the map for each level
class Map {
private:
    Tile grid[6][6];               // 6x6 grid representing the map
    int playerRow;                 // Current row of the player
    int playerCol;                 // Current column of the player
    vector<string> enemyEmojis;   // Visual emojis for enemies
    vector<string> itemEmojis;    // Visual emojis for items
    int prevRow;                   // Previous row before last movement
    int prevCol;                   // Previous column before last movement

    // Returns a random emoji representing an enemy
    string getRandomEnemyEmoji() const;

public:
    Map();                         // Constructor
    ~Map();                        // Destructor

    // Displays the minimap showing only visible surroundings
    void displayMap(const Character& player, int currentLevel) const;

    // Displays the full map with all tiles revealed
    void displayFullMap(const Character& player, int currentLevel) const;

    // Populates the current level with enemies and items
    void populateLevel(int level, const string& playerClass);

    // Handles player movement input and interactions
    void movePlayer(char direction, Character& player, Inventory& inventory);

    // Retrieves the player's current coordinates
    void getPlayerPosition(int& row, int& col) const;

    // Returns a pointer to the enemy at the player's position (if any)
    Enemy* getEnemyAtPlayer();

    // Clears the enemy at the player's current position
    void clearEnemyAtPlayer();

    // Returns true if all enemies on the map have been defeated
    bool allEnemiesDefeated() const;

    // Resets the player's position to the default
    void resetPlayerPosition();

    // Moves the player to a specific coordinate
    void movePlayerTo(int row, int col);

    // Returns the emoji representation of the player
    string getPlayerEmoji(const Character& player) const;

    // Returns the player to their previous position (Run)
    void fleeToPreviousPosition();

    // Places an item at the player's current position with a given emoji
    bool placeItemAtPlayerPosition(const Item& item, const string& emoji);

    // Returns a random emoji for an item
    string getRandomItemEmoji() const;

    // Returns an emoji corresponding to a specific item type
    string getEmojiForItem(const Item& item) const;

    // Returns the tile at the given coordinates
    Tile getTile(int r, int c) const {
        return grid[r][c];
    }

    // Returns a reference to the tile at the given coordinates
    Tile& getMutableTile(int r, int c) {
        return grid[r][c];
    }

    // Sets the player's current position
    void setPlayerPosition(int r, int c) {
        playerRow = r;
        playerCol = c;
    }

    // Gets the player's current row
    int getPlayerRow() const {
        return playerRow;
    }

    // Gets the player's current column
    int getPlayerCol() const {
        return playerCol;
    }

    // Gets the player's previous row
    int getPrevRow() const;

    // Gets the player's previous column
    int getPrevCol() const;

    // Sets the previous position
    void setPrevPosition(int row, int col);
};

#endif // MAP_H_INCLUDED
