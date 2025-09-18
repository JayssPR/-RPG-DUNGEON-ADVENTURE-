#include "map.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constructor: initializes map and places the player at the start
Map::Map() {
    resetPlayerPosition();  // Set player at initial position (bottom-right)
    enemyEmojis = {"[! !]"};
    itemEmojis = {"[ F ]", "[ H ]", "[ E ]", "[ $ ]"};

    // Initialize each cell in the grid
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 6; c++) {
            grid[r][c].symbol = "[   ]";
            grid[r][c].hasEnemy = false;
            grid[r][c].hasItem = false;
            grid[r][c].enemy = nullptr;
        }
    }

    grid[0][0].symbol = "[/-\\]";  // Goal cell
    grid[playerRow][playerCol].symbol = getPlayerEmoji(Character());  // Player icon
    prevRow = playerRow;
    prevCol = playerCol;
}

// Destructor: cleans up dynamically allocated enemy objects
Map::~Map() {
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 6; c++) {
            if (grid[r][c].enemy != nullptr) {
                delete grid[r][c].enemy;
            }
        }
    }
}

// Display a mini map showing only the current row
void Map::displayMap(const Character& player, int currentLevel) const {
    cout << "\n+============= MINI MAP =============+" << endl;
    cout << "| Dungeon Level: " << currentLevel;
    int levelSpaces = 34 - to_string(currentLevel).length();
    for (int i = 0; i < levelSpaces; i++) cout << " ";
    cout << "|\n";

    cout << "| HP: " << player.getHPBar() << "   Level: " << player.getLevel();
    int spaces = 34 - player.getHPBar().length() - to_string(player.getLevel()).length();
    for (int i = 0; i < spaces; i++) cout << " ";
    cout << "|\n";

    cout << "+------------------------------------+" << endl;
    cout << "     A     B     C     D     E     F" << endl;
    cout << "   +-----+-----+-----+-----+-----+-----+" << endl;

    char rowLabel = 'G' + playerRow;
    cout << " " << rowLabel << " |";
    for (int c = 0; c < 6; c++) {
        if (c == playerCol)
            cout << getPlayerEmoji(player) << "|";
        else
            cout << grid[playerRow][c].symbol << "|";
    }
    cout << "\n   +-----+-----+-----+-----+-----+-----+" << endl;

    // Legend for the symbols
    cout << "\nLegend:\n";
    cout << " " << getPlayerEmoji(player) << " Player (based on HP)\n";
    cout << " [! !] Enemy\n";
    cout << " [/-\\] Goal\n";
    cout << " [ F ] Food   [ H ] Heal   [ E ] Energy   [ $ ] Gold\n" << endl;
}

// Display the entire dungeon map
void Map::displayFullMap(const Character& player, int currentLevel) const {
    cout << "\n+============ FULL MAP =============+" << endl;
    cout << "| Dungeon Level: " << currentLevel;
    int levelSpaces = 34 - to_string(currentLevel).length();
    for (int i = 0; i < levelSpaces; i++) cout << " ";
    cout << "|\n";

    cout << "| HP: " << player.getHPBar() << "   Level: " << player.getLevel();
    int spaces = 34 - player.getHPBar().length() - to_string(player.getLevel()).length();
    for (int i = 0; i < spaces; i++) cout << " ";
    cout << "|\n";

    cout << "+------------------------------------+" << endl;
    cout << "     A     B     C     D     E     F" << endl;
    cout << "   +-----+-----+-----+-----+-----+-----+" << endl;

    for (int r = 0; r < 6; r++) {
        char rowLabel = 'G' + r;
        cout << " " << rowLabel << " |";
        for (int c = 0; c < 6; c++) {
            if (r == playerRow && c == playerCol)
                cout << getPlayerEmoji(player) << "|";
            else
                cout << grid[r][c].symbol << "|";
        }
        cout << "\n   +-----+-----+-----+-----+-----+-----+" << endl;
    }

    cout << "\nLegend:\n";
    cout << " " << getPlayerEmoji(player) << " Player (based on HP)\n";
    cout << " [! !] Enemy\n";
    cout << " [/-\\] Goal\n";
    cout << " [ F ] Food   [ H ] Heal   [ E ] Energy   [ $ ] Gold\n" << endl;
}

// Resets the player's position to the starting cell
void Map::resetPlayerPosition() {
    playerRow = 5;
    playerCol = 5;
}
// Return player to the previous position (used for Run)
void Map::fleeToPreviousPosition() {
    if (grid[playerRow][playerCol].hasEnemy) {
        if (grid[playerRow][playerCol].enemy != nullptr) {
            grid[playerRow][playerCol].symbol = "[! !]";
        }
    } else if (grid[playerRow][playerCol].hasItem) {
        grid[playerRow][playerCol].symbol = getEmojiForItem(grid[playerRow][playerCol].item);
    } else {
        grid[playerRow][playerCol].symbol = "[   ]";
    }

    playerRow = prevRow;
    playerCol = prevCol;
    grid[playerRow][playerCol].symbol = "[*_*]";
}


// Populates the level with enemies and items based on difficulty
void Map::populateLevel(int level, const string& playerClass) {
    // Clear the grid
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 6; c++) {
            grid[r][c].symbol = "[   ]";
            grid[r][c].hasEnemy = false;
            grid[r][c].hasItem = false;
            if (grid[r][c].enemy != nullptr) {
                delete grid[r][c].enemy;
                grid[r][c].enemy = nullptr;
            }
        }
    }

    resetPlayerPosition();
    prevRow = playerRow;
    prevCol = playerCol;
    grid[0][0].symbol = "[/-\\]";
    grid[playerRow][playerCol].symbol = getPlayerEmoji(Character());

    // Determine number of elements (scaled with level)
    int totalCells = 36;
    int numElements = (totalCells * (10 + (level - 1) * 5) + 99) / 100;
    if (numElements < 1){
       numElements = 1;
    }

    int enemies = numElements * 50 / 100;
    int food = numElements * 30 / 100;
    int healing = numElements * 10 / 100;
    int energy = numElements * 10 / 100;
    int egold = numElements - (enemies + food + healing + energy);

    // Build element list
    vector<string> elements;
    for (int i = 0; i < enemies; i++) {
        elements.push_back("ENEMY");
    }
    for (int i = 0; i < food; i++) {
        elements.push_back("FOOD");
    }
    for (int i = 0; i < healing; i++) {
        elements.push_back("HEALING");
    }
    for (int i = 0; i < energy; i++) {
        elements.push_back("ENERGY");
    }
    for (int i = 0; i < egold; i++) {
        elements.push_back("EGOLD");
    }

    for (int i = elements.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(elements[i], elements[j]);
    }

    // Place each element randomly
    int placed = 0;
    while (placed < elements.size()) {
        int r = rand() % 6;
        int c = rand() % 6;

        if ((r != playerRow || c != playerCol) && !(r == 0 && c == 0) &&
            !grid[r][c].hasEnemy && !grid[r][c].hasItem) {

            string elem = elements[placed];

            if (elem == "ENEMY") {
                int enemyLevel = (level == 1) ? (rand() % 2 + 1) : (level - 1 + rand() % 3);
                if (enemyLevel < 1) enemyLevel = 1;
                grid[r][c].enemy = new Enemy(playerClass, enemyLevel);
                grid[r][c].hasEnemy = true;
                grid[r][c].symbol = "[! !]";
            } else {
                ItemType type;
                string name;

                if (elem == "HEALING") {
                    type = HEALING_POTION; name = "Healing Potion";
                } else if (elem == "ENERGY") {
                    type = ENERGY_POTION; name = "Energy Potion";
                } else if (elem == "EGOLD") {
                    type = EGOLD; name = "e-Gold";
                } else {
                    type = FOOD; name = "Food";
                }

                Item item(name, type);
                grid[r][c].item = item;
                grid[r][c].hasItem = true;
                grid[r][c].symbol = getEmojiForItem(item);
            }

            placed++;
        }
    }
}

// Move the player on the map, collect items, detect enemies
void Map::movePlayer(char direction, Character& player, Inventory& inventory) {
    prevRow = playerRow;
    prevCol = playerCol;

    // Restore previous cell's symbol
    if (grid[playerRow][playerCol].hasItem) {
        grid[playerRow][playerCol].symbol = getEmojiForItem(grid[playerRow][playerCol].item);
    } else {
        grid[playerRow][playerCol].symbol = "[   ]";
    }

    // Perform move if possible
    if ((direction == 'W' || direction == 'w') && playerRow > 0) {
        playerRow--;
    } else if ((direction == 'S' || direction == 's') && playerRow < 5) {
        playerRow++;
    } else if ((direction == 'A' || direction == 'a') && playerCol > 0) {
        playerCol--;
    } else if ((direction == 'D' || direction == 'd') && playerCol < 5) {
        playerCol++;
    } else {
        cout << "You can't move in that direction!" << endl;
    }

    Tile& tile = grid[playerRow][playerCol];

    // Pick up item
    if (tile.hasItem) {
        inventory.addItem(tile.item, player);
        cout << "You found a " << tile.item.getName() << "!" << endl;
        tile.hasItem = false;
    }

    // Update symbol
    if (!(playerRow == 0 && playerCol == 0)) {
        tile.symbol = getPlayerEmoji(player);
    } else {
        tile.symbol = "[/-\\]";
    }

    // Notify if enemy is encountered
    if (tile.hasEnemy && tile.enemy != nullptr && tile.enemy->isAlive()) {
        cout << "You encountered a level " << tile.enemy->getLevel()
             << " " << tile.enemy->getType() << "!" << endl;
    }
}

// Get the enemy at the player’s position
Enemy* Map::getEnemyAtPlayer() {
    return grid[playerRow][playerCol].enemy;
}

// Remove enemy from player’s current tile
void Map::clearEnemyAtPlayer() {
    Tile& tile = grid[playerRow][playerCol];
    if (tile.enemy != nullptr) {
        delete tile.enemy;
        tile.enemy = nullptr;
        tile.hasEnemy = false;
        tile.symbol = "[   ]";
    }
}

// Output player coordinates
void Map::getPlayerPosition(int& row, int& col) const {
    row = playerRow;
    col = playerCol;
}

// Check if all enemies on the map are defeated
bool Map::allEnemiesDefeated() const {
    bool allDefeated = true;

    for (int r = 0; r < 6 && allDefeated; r++) {
        for (int c = 0; c < 6 && allDefeated; c++) {
            if (grid[r][c].hasEnemy && grid[r][c].enemy != nullptr && grid[r][c].enemy->isAlive()) {
                allDefeated = false;
            }
        }
    }

    return allDefeated;
}


// Move the player directly to a specific position
void Map::movePlayerTo(int row, int col) {
    grid[playerRow][playerCol].symbol = "[   ]";
    playerRow = row;
    playerCol = col;
    grid[playerRow][playerCol].symbol = "[*_*]";
}

// Determine player emoji based on current HP percentage
string Map::getPlayerEmoji(const Character& player) const {
    int hp = player.getHP();
    int maxHp = player.getMaxHP();
    string emoji = "[^_^]";  // default

    if (maxHp <= 0) {
        emoji = "[x_x]";
    } else {
        int percent = (hp * 100) / maxHp;
        if (percent <= 75) emoji = "[-_-]";
        if (percent <= 40) emoji = "[>_<]";
        if (percent <= 10) emoji = "[x_x]";
    }

    return emoji;
}


// Place an item at the current player position
bool Map::placeItemAtPlayerPosition(const Item& item, const string& emoji) {
    Tile& tile = grid[playerRow][playerCol];
    bool placed = false;

    if (!tile.hasItem && !tile.hasEnemy) {
        tile.item = item;
        tile.hasItem = true;
        tile.symbol = emoji;
        placed = true;
    }

    return placed;
}


// Return the emoji that corresponds to the item type
string Map::getEmojiForItem(const Item& item) const {
    string emoji;
    ItemType type = item.getType();

    if (type == FOOD)
        emoji = "[ F ]";
    else if (type == HEALING_POTION)
        emoji = "[ H ]";
    else if (type == ENERGY_POTION)
        emoji = "[ E ]";
    else if (type == EGOLD)
        emoji = "[ $ ]";
    else
        emoji = "[ ? ]";

    return emoji;
}


// Get previous row/column values
int Map::getPrevRow() const {
    return prevRow;
    }
int Map::getPrevCol() const {
    return prevCol;
    }

// Set previous player position (for custom control)
void Map::setPrevPosition(int row, int col) {
    prevRow = row;
    prevCol = col;
}
