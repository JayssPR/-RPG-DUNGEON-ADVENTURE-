#include "game.h"

// Constructor initializes the game state
Game::Game() {
    currentLevel = 1;
    totalLevels = 3;
    gameOver = false;
    fledBattle = false;
}

// Starts the main game loop and handles menu navigation
void Game::start() {
    bool exitGame = false;
    while (!exitGame) {
        showMainMenu();  // Display the main menu
        char choice = getValidatedCharInput("123");

        if (choice == '1') {
            startNewGame(); // Start a new game
        } else if (choice == '2') {
            loadExistingGame(); // Load a saved game
        } else {
            cout << "Goodbye!" << endl;
            exitGame = true; // Exit the game loop
        }
    }
}

// Handles setup for a new game session
void Game::startNewGame() {
    gameOver = false;
    player.createCharacter(); // Create a new player character
    inventory = Inventory();  // Initialize inventory

    cout << "Enter the number of levels to play (5 to 10): ";
    totalLevels = getValidatedInput(5, 10); // Ask for total levels

    currentLevel = 1;
    map.populateLevel(currentLevel, player.getClass()); // Populate map for level
    gameLoop(); // Enter main game loop
}

// Attempts to load a previously saved game
void Game::loadExistingGame() {
    gameOver = false;
    if (loadGame()) {
        gameLoop(); // Start game loop if loading succeeded
    } else {
        cout << "Failed to load game." << endl;
    }
}

// Saves the current game state to a file
bool Game::saveGame() {
    bool success = true;
    string filename;
    cout << "Enter filename to save (example: save.txt): ";
    cin >> filename;

    // Check if the file already exists
    ifstream testFile(filename);
    if (testFile.good()) {
        cout << "File already exists. Overwrite? (Y/N): ";
        char confirm = getValidatedCharInput("YN");
        if (confirm == 'N') {
            cout << "Canceled saving." << endl;
            success = false;
        }
    }

    // Write data to the file
    if (success) {
        ofstream outFile(filename);
        if (!outFile) {
            cout << "Error saving the game." << endl;
            success = false;
        } else {
            // Save player attributes
            outFile << player.getName() << " " << player.getClass() << " " << player.getLevel() << " "
                    << player.getHP() << " " << player.getMaxHP() << " "
                    << player.getStrength() << " " << player.getDexterity() << " " << player.getConstitution() << " "
                    << player.getIntelligence() << " " << player.getWisdom() << " " << player.getCharisma() << " "
                    << player.getXP() << " " << inventory.getGold() << endl;

            // Save inventory
            outFile << inventory.getItemCount() << endl;
            for (int i = 0; i < inventory.getItemCount(); i++) {
                outFile << inventory.getItem(i).getName() << " " << inventory.getItem(i).getType() << endl;
            }

            // Save player position
            int row; //CHANGE
            int col; //CHANGE
            map.getPlayerPosition(row, col);
            outFile << row << " " << col << endl;
            outFile << map.getPrevRow() << " " << map.getPrevCol() << endl;
            outFile << currentLevel << endl;

            // Save map state (tiles)
            for (int r = 0; r < 6; r++) {
                for (int c = 0; c < 6; c++) {
                    Tile tile = map.getTile(r, c);
                    outFile << tile.symbol << " " << tile.hasItem << " "
                            << tile.item.getName() << " " << tile.item.getType() << " " << tile.hasEnemy;
                    if (tile.hasEnemy && tile.enemy != nullptr) {
                        outFile << " " << tile.enemy->getType()
                                << " " << tile.enemy->getLevel()
                                << " " << tile.enemy->getHP();
                    }
                    outFile << endl;
                }
            }

            cout << "Game saved successfully!" << endl;
        }
    }

    return success;
}

// Loads a saved game from file
bool Game::loadGame() {
    // Prompt the user to enter the filename to load the saved game
    string filename;
    cout << "Enter filename to load (example: save.txt): ";
    cin >> filename;

    bool success = true;

    // Attempt to open the save file
    ifstream inFile(filename);
    if (!inFile) {
        cout << "Save file not found." << endl;
        success = false;
    }

    if (success) {
        // Variables to store the player’s state
        string name;
        string playerClass;//CHANGE
        int level;
        int hp; //CHANGE
        int maxHp;//CHANGE
        int strength;//CHANGE
        int dexterity; //CHANGE
        int constitution;//CHANGE
        int intelligence;//CHANGE
        int wisdom; //CHANGE
        int charisma; //CHANGE
        int xp; //CHANGE
        int gold;  //CHANGE

        // Read player stats from the file
        inFile >> name;
        inFile >> playerClass;//CHANGE
        inFile >> level;//CHANGE
        inFile >> hp;//CHANGE
        inFile >> maxHp;//CHANGE
        inFile >> strength;//CHANGE
        inFile >> dexterity;//CHANGE
        inFile >> constitution;//CHANGE
        inFile >> intelligence;//CHANGE
        inFile >> wisdom;//CHANGE
        inFile >> charisma;//CHANGE
        inFile >> xp;//CHANGE
        inFile >> gold;//CHANGE

        if (inFile.fail()) {
            cout << "Corrupted save file. Cannot load." << endl;
            success = false;
        } else {
            // Restore the player's full state and initialize inventory
            player.restoreFullState(name, playerClass, level, hp, maxHp, strength, dexterity, constitution,
                                    intelligence, wisdom, charisma, xp);
            inventory = Inventory();
            inventory.addGold(gold);

            // Read and load inventory items
            int itemCount;
            inFile >> itemCount;
            for (int i = 0; i < itemCount && success; i++) {
                string itemName;
                int itemType;
                inFile >> itemName >> itemType;
                if (inFile.fail()) {
                    success = false;
                    cout << "Error loading inventory item." << endl;
                } else {
                    inventory.addItemDirectly(Item(itemName, static_cast<ItemType>(itemType)));
                }
            }

            // Read player's current and previous map positions
            int row;
            int col;//CHANGE
            int prevRow;//CHANGE
            int prevCol;//CHANGE
            inFile >> row;
            inFile >> col;//CHANGE
            inFile >> prevRow;//CHANGE
            inFile >> prevCol;//CHANGE
            inFile >> currentLevel;//CHANGE
            if (inFile.fail()) {
                cout << "Error loading player/map positions." << endl;
                success = false;
            } else {
                map.setPlayerPosition(row, col);
                map.setPrevPosition(prevRow, prevCol);
            }

            // Read tile data for a 6x6 map grid
            bool tileReadSuccess = true;
            for (int r = 0; r < 6 && tileReadSuccess && success; r++) {
                for (int c = 0; c < 6 && tileReadSuccess && success; c++) {
                    string symbol;
                    string itemName;//CHANGE
                    string enemyType;//CHANGE
                    int hasItemInt;
                    int itemTypeInt;//CHANGE
                    int hasEnemyInt;//CHANGE
                    int enemyLevel = 0;//CHANGE
                    int enemyHP = 0;//CHANGE

                    // Read tile symbol (up to ']')
                    inFile.ignore();
                    getline(inFile, symbol, ']');
                    symbol += "]";

                    // Read item/enemy presence and basic item data
                    inFile >> hasItemInt;
                    inFile >> itemName;//CHANGE
                    inFile >> itemTypeInt;//CHANGE
                    inFile >> hasEnemyInt;//CHANGE

                    if (inFile.fail()) {
                        cout << "Error reading tile data." << endl;
                        tileReadSuccess = false;
                        success = false;
                    } else {
                        Tile& tile = map.getMutableTile(r, c);
                        tile.symbol = symbol;
                        tile.hasItem = (hasItemInt == 1);
                        tile.hasEnemy = (hasEnemyInt == 1);

                        // If tile has an item, create and assign it
                        if (tile.hasItem) {
                            tile.item = Item(itemName, static_cast<ItemType>(itemTypeInt));
                        }

                        // If tile has an enemy, read enemy data and create the enemy
                        if (tile.hasEnemy) {
                            inFile >> enemyType >> enemyLevel >> enemyHP;
                            if (inFile.fail()) {
                                cout << "Error loading enemy data." << endl;
                                tileReadSuccess = false;
                                success = false;
                            } else {
                                Enemy* enemy = new Enemy(enemyType, enemyLevel);
                                // Reduce enemy HP to saved value
                                while (enemy->getHP() > enemyHP) enemy->takeDamage(1);
                                tile.enemy = enemy;
                            }
                        } else {
                            tile.enemy = nullptr;
                        }
                    }
                }
            }
        }
    }

    // Show success message if everything was loaded correctly
    if (success) {
        cout << "Game loaded successfully!" << endl;
    }

    return success;
}


// Main game loop for gameplay and input handling
void Game::gameLoop() {
    while (!gameOver) {
        map.displayMap(player, currentLevel); // Show current view of map

        Enemy* enemy = map.getEnemyAtPlayer();


        bool inCombat = false; //CHANGE
        if (enemy != nullptr){
            if (enemy->isAlive()){
                inCombat = true;
            }
        }



        if (inCombat) {
            // Combat mode
            cout << "[F] Fight | [R] Run" << endl;
            cout << "Action: ";
            char combatChoice = getValidatedCharInput("FR");

            if (combatChoice == 'F') {
                battle(*enemy); // Start a battle
                if (!player.isAlive()) {
                    cout << "You have died. Game over." << endl;
                    gameOver = true;
                } else {
                    fledBattle = false;
                }
            } else if (combatChoice == 'R') {
                // Player flees from combat
                cout << "You drop your weapon and run before the fight begins..." << endl;
                int dmg = enemy->attack();
                cout << "!! The enemy strikes you for " << dmg << " damage!" << endl;
                player.takeDamage(dmg);
                map.fleeToPreviousPosition();
                fledBattle = true;
            }

        } else {
            // Movement and menu options
            cout << "[W] Up | [S] Down | [A] Left | [D] Right | [I] Inventory | [C] Character | [M] Map | [Q] Quit" << endl;
            cout << "Move: ";
            char input = getValidatedCharInput("WASDICMQ");

            if (input == 'Q') {
                cout << "Do you want to save the game before quitting? (Y/N): ";
                char saveChoice = getValidatedCharInput("YN");
                if (saveChoice == 'Y') {
                    saveGame();
                }
                gameOver = true;
            } else if (input == 'I') {
                inventory.showInventory();
                // Item usage and dropping
                if (inventory.getItemCount() > 0) {
                    cout << "Do you want to use an item? (Y/N): ";
                    char useChoice = getValidatedCharInput("YN");
                    if (useChoice == 'Y') {
                        cout << "Enter the item number to use: ";
                        int itemChoice = getValidatedInput(1, inventory.getItemCount());
                        inventory.useItem(itemChoice, player);
                    }
                    cout << "Do you want to drop an item? (Y/N): ";
                    char dropChoice = getValidatedCharInput("YN");
                    if (dropChoice == 'Y') {
                        cout << "Enter the item number to drop: ";
                        int dropIndex = getValidatedInput(1, inventory.getItemCount());
                        Item itemToDrop;
                        bool dropped = inventory.dropItem(dropIndex, itemToDrop);
                        string emoji = map.getEmojiForItem(itemToDrop);
                        bool placed = map.placeItemAtPlayerPosition(itemToDrop, emoji);
                        if (placed) {
                            cout << "You dropped " << itemToDrop.getName() << " on the ground.\n";
                        } else {
                            cout << "You cannot drop the item here. Tile is occupied.\n";
                            inventory.addItem(itemToDrop, player);
                        }
                    }
                }
            } else if (input == 'C') {
                player.showStats();
            } else if (input == 'M') {
                map.displayFullMap(player, currentLevel);
            } else {
                // Move player and check for exit
                map.movePlayer(input, player, inventory);
                int row, col;
                map.getPlayerPosition(row, col);
                bool atExit = (row == 0 && col == 0);

                if (atExit) {
                    // Check for remaining enemies or items
                    bool enemiesLeft = !map.allEnemiesDefeated();
                    bool itemsLeft = false;
                    for (int r = 0; r < 6 && !itemsLeft; r++) {
                        for (int c = 0; c < 6 && !itemsLeft; c++) {
                            if (map.getTile(r, c).hasItem) {
                                itemsLeft = true;
                            }
                        }
                    }

                    cout << "\n=== Exit Tile Reached ===" << endl;
                    if (enemiesLeft){
                        cout << "Warning: Some enemies remain on this level." << endl;
                    }
                    if (itemsLeft){
                        cout << "Note: There are still items left to collect." << endl;
                    }

                    // Optionally save game
                    cout << "Do you want to save your game before proceeding? (Y/N): ";
                    char saveChoice = getValidatedCharInput("YN");
                    if (saveChoice == 'Y') {
                        saveGame();
                    }

                    // Optionally proceed
                    cout << "Proceed to the next level? (Y/N): ";
                    char proceedChoice = getValidatedCharInput("YN");
                    if (proceedChoice == 'Y') {
                        currentLevel++;
                        if (currentLevel > totalLevels) {
                            // Game won
                            cout << "\n+=========================================+" << endl;
                            cout << "|        CONGRATULATIONS, HERO!           |" << endl;
                            cout << "+-----------------------------------------+" << endl;
                            cout << "| You have bravely conquered all levels   |" << endl;
                            cout << "| of the dungeon and emerged victorious!  |" << endl;
                            cout << "+=========================================+\n" << endl;

                            cout << "Would you like to save your victory? (Y/N): ";
                            char saveWinChoice = getValidatedCharInput("YN");
                            if (saveWinChoice == 'Y') {
                                saveGame();
                            }

                            cout << "Would you like to view your final character stats? (Y/N): ";
                            char statsChoice = getValidatedCharInput("YN");
                            if (statsChoice == 'Y') {
                                player.showStats();
                            }

                            cout << "\nThanks for playing! Until the next adventure..." << endl;
                            gameOver = true;
                        } else {
                            // Next level setup
                            cout << "Proceeding to level " << currentLevel << "..." << endl;
                            map.populateLevel(currentLevel, player.getClass());
                        }
                    } else {
                        cout << "You decide to stay and explore more." << endl;
                    }
                }
            }
        }
    }
}

void Game::battle(Enemy& enemy) {
    // Display battle header and basic information
    cout << "\n+=========== BATTLE START ===========+" << endl;
    cout << " Enemy Type: " << enemy.getType() << " (Level " << enemy.getLevel() << ")" << endl;
    cout << " " << enemy.getHPBar() << endl;
    cout << " Your HP: " << player.getHPBar() << endl;
    cout << "+====================================+\n";

    // Initialize battle state variables
    bool battleOver = false;
    bool playerVictory = false;
    int itemsUsed = 0;
    fledBattle = false;

    // Main battle loop
    while (player.isAlive() && enemy.isAlive() && !battleOver) {
        // Display action options
        cout << "\n[A]ttack  [U]se Item  [R]un" << endl;
        cout << "Choose action: ";
        char action = getValidatedCharInput("AUR");

        cout << "\n------------------------------------" << endl;

        if (action == 'A') {
            // Handle player attack turn
            executePlayerTurn(enemy, battleOver, playerVictory, itemsUsed);

            // Enemy counterattacks if still alive and battle not over
            if (!battleOver && enemy.isAlive()) {
                executeEnemyTurn(enemy, battleOver, playerVictory);
            }

        } else if (action == 'U') {
            // Use item from inventory
            inventory.showInventory();
            if (inventory.getItemCount() > 0) {
                cout << "Enter item number to use: ";
                int idx = getValidatedInput(1, inventory.getItemCount());
                inventory.useItem(idx, player);
                itemsUsed++;
            } else {
                cout << "You have no usable items." << endl;
            }

        } else if (action == 'R') {
            // Player chooses to flee; enemy attacks
            cout << "You drop your weapon and surrender..." << endl;
            int dmg = enemy.attack();
            cout << "!! The enemy strikes you mercilessly for " << dmg << " damage!" << endl;
            player.takeDamage(dmg);
            battleOver = true;
            playerVictory = false;
        }

        // Show HP warnings based on current health
        showHPWarnings();

        // Display updated HP bars
        cout << "\nEnemy HP:  " << enemy.getHPBar() << endl;
        cout << "Your HP:   " << player.getHPBar() << endl;
        cout << "------------------------------------";

        // Chance of player surrendering if HP is low
        if (!battleOver) { //CHANGE
            if (player.getHP() < player.getMaxHP() / 2) {
                if (rollDice(100) <= 20) {
                    cout << "\nYou feel hopeless and drop your weapon...\n";
                    cout << "** You surrender! The enemy wins the battle. **" << endl;
                    int dmg = enemy.attack();
                    cout << "!! The enemy strikes you for " << dmg << " damage!" << endl;
                    player.takeDamage(dmg);
                    battleOver = true;
                    playerVictory = false;
                }
            }
        }

        // Chance of enemy surrendering if HP is low
        if (!battleOver) { //CHANGE
            if (enemy.getHP() < enemy.getMaxHP() / 2) {
                if (rollDice(100) <= 20) {
                    cout << "** The enemy surrenders! You win the battle! **" << endl;
                    int xpGained = 20 * enemy.getLevel();
                    cout << "You gained " << xpGained << " XP!" << endl;
                    player.gainXP(xpGained);
                    enemy.takeDamage(enemy.getHP());
                    playerVictory = true;
                    battleOver = true;
                }
            }
        }
    }

    // Grant XP if player wins
    if (player.isAlive()) { //CHANGE
        if (!fledBattle) {
            if (enemy.getHP() <= 0) {
                playerVictory = true;
                int xpGained = 20 * enemy.getLevel();
                cout << "\n** You defeated the enemy! +" << xpGained << " XP! **" << endl;
                player.gainXP(xpGained);
            }
        }
    }

    // Display battle summary
cout << "\n\n+========= BATTLE SUMMARY =========+" << endl;
cout << "Enemy: " << enemy.getType() << " (Level " << enemy.getLevel() << ")" << endl;
cout << "Result: ";
if (playerVictory) {
    cout << "Victory";
} else if (fledBattle) {
    cout << "Run";
} else {
    cout << "Defeated";
}
cout << endl;

// Compute XP gained without using ternary operator
int xpGained;
if (playerVictory) {
    xpGained = 20 * enemy.getLevel();
} else {
    xpGained = 0;
}

cout << "XP Gained: " << xpGained << endl;
cout << "HP Remaining: " << player.getHP() << "/" << player.getMaxHP() << endl;
cout << "Items Used: " << itemsUsed << endl;
cout << "+==================================+\n";
}
// Handles initiative and attack order for player and enemy
void Game::executePlayerTurn(Enemy& enemy, bool& battleOver, bool& playerVictory, int& itemsUsed) {
    int playerInit;
    int enemyInit;

    // Roll initiative until there's a winner
    do {
        playerInit = rollDice(20) + player.getDexterity();
        enemyInit = rollDice(20) + enemy.getDexterity();
    } while (playerInit == enemyInit);

    cout << "Initiative - Player: " << playerInit << " | Enemy: " << enemyInit << endl;

    bool enemyTurnFirst = (enemyInit < playerInit);
    bool enemyAlive = enemy.isAlive();
    bool playerAlive = player.isAlive();

    // Execute attack sequence depending on initiative
    if (enemyTurnFirst) {
        performEnemyAttack(enemy);
        playerAlive = player.isAlive();

        if (playerAlive) {
            performPlayerAttack(enemy);
            enemyAlive = enemy.isAlive();
        }
    } else {
        performPlayerAttack(enemy);
        enemyAlive = enemy.isAlive();

        if (enemyAlive) {
            performEnemyAttack(enemy);
            playerAlive = player.isAlive();
        }
    }

    // Determine if battle ended
    if (!enemyAlive) {
        playerVictory = true;
        battleOver = true;
    } else if (!playerAlive) {
        battleOver = true;
    }

    // Update any temporary player buffs
    player.updateEnergyBuff();
}

// Executes a simple enemy turn outside of initiative
void Game::executeEnemyTurn(Enemy& enemy, bool& battleOver, bool& playerVictory) {
    int dmg = enemy.attack();
    cout << "!! Enemy attacks and deals " << dmg << " damage!" << endl;
    player.takeDamage(dmg);
}

// Enemy attacks player
void Game::performEnemyAttack(Enemy& enemy) {
    int dmg = enemy.attack();
    cout << "!! Enemy attacks and deals " << dmg << " damage!" << endl;
    player.takeDamage(dmg);
}

// Player attacks enemy with class-based logic
void Game::performPlayerAttack(Enemy& enemy) { //CHANGE
    int roll = rollDice(20);
    int maxDmg = 6;

    string playerClass = player.getClass();

    if (playerClass == "Warrior") {
        maxDmg = 12;
    } else if (playerClass == "Rogue") {
        maxDmg = 10;
    } else if (playerClass == "Cleric") {
        maxDmg = 8;
    } else if (playerClass == "Wizard") {
        maxDmg = 6;
    }

    if (roll == 20) {
        cout << "Critical hit! You deal maximum damage!" << endl;
        enemy.takeDamage(maxDmg);
    } else if (roll == 1) {
        int selfDmg = player.attack();
        cout << "Epic fail! You hit yourself for " << selfDmg << " damage!" << endl;
        player.takeDamage(selfDmg);
    } else if (roll <= enemy.getDexterity()) {
        cout << "Enemy evades your attack!" << endl;
    } else {
        int dmg = player.attack();
        cout << "You hit the enemy for " << dmg << " damage." << endl;
        enemy.takeDamage(dmg);
    }
}

// Displays HP-related warnings for the player
void Game::showHPWarnings() {
    int hp = player.getHP();
    int maxHp = player.getMaxHP();
    int percent = (hp * 100) / maxHp;

    if (percent <= 10) {
        cout << "CRITICAL WARNING: HP below 10%!" << endl;
    } else if (percent <= 25) {
        cout << "Warning: HP below 25%!" << endl;
    } else if (percent <= 50) {
        cout << "Caution: HP below 50%!" << endl;
    } else if (percent <= 75) {
        cout << "HP below 75%. Stay alert." << endl;
    }
}


