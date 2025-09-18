#include "character.h"
#include "utilities.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constructor: initializes default character attributes
Character::Character() {
    level = 1;
    xp = 0;
    buffed = false;
    buffTurns = 0;
}

// Function to create a new character with input validation
void Character::createCharacter() {
    bool isValid = false;
    while (!isValid) {
        isValid = true;
        cout << "Enter your character's name: ";
        getline(cin, name);

        // Ensure name is not empty
        if (name.empty()) {
            cout << "Name cannot be empty. Please enter a valid name." << endl;
            isValid = false;
        }

        // Check for invalid characters (only letters and spaces allowed)
        if (isValid) {
            for (size_t i = 0; i < name.size(); i++) {
                if (!(isalpha(name[i]) || name[i] == ' ')) {
                    cout << "Invalid name. Please use only letters and spaces." << endl;
                    isValid = false;
                    i = name.size(); // Exit loop early
                }
            }
        }
    }

    // Character class selection
    cout << "\n+=====================================+" << endl;
    cout << "|       Choose a Character Type       |" << endl;
    cout << "+-------------------------------------+" << endl;
    cout << "|  [W] Warrior  - Strong melee hero   |" << endl;
    cout << "|  [R] Rogue    - Agile and cunning   |" << endl;
    cout << "|  [C] Cleric   - Healer and support  |" << endl;
    cout << "|  [Z] Wizard   - Powerful spellcaster|" << endl;
    cout << "+=====================================+" << endl;
    cout << "Enter your choice: ";
    getline(cin, charClass);

    // Validate class input
    while (charClass != "W" && charClass != "w" &&
           charClass != "R" && charClass != "r" &&
           charClass != "C" && charClass != "c" &&
           charClass != "Z" && charClass != "z") {
        cout << "Invalid class. Please choose from Warrior, Rogue, Cleric, or Wizard: ";
        getline(cin, charClass);
    }

    // Normalize input to class name
    if (charClass == "W" || charClass == "w") charClass = "Warrior";
    else if (charClass == "R" || charClass == "r") charClass = "Rogue";
    else if (charClass == "C" || charClass == "c") charClass = "Cleric";
    else if (charClass == "Z" || charClass == "z") charClass = "Wizard";

    calculateStats(); // Roll stats for the character
}

// Generate stats and calculate HP
void Character::calculateStats() {
    strength = rollStat();
    dexterity = rollStat();
    constitution = rollStat();
    intelligence = rollStat();
    wisdom = rollStat();
    charisma = rollStat();
    maxHP = calculateHP();
    currentHP = maxHP;
}

// Calculate HP based on class with random bonus
int Character::calculateHP() const {
    unsigned seed = time(0);
    srand(seed);
    int hp = 0;
    if (charClass == "Warrior") hp = (rand() % 12 + 1) + 12;
    else if (charClass == "Rogue") hp = (rand() % 10 + 1) + 10;
    else if (charClass == "Cleric") hp = (rand() % 8 + 1) + 8;
    else if (charClass == "Wizard") hp = (rand() % 6 + 1) + 6;
    else hp = (rand() % 8 + 1) + 8;
    return hp;
}

// Getter for current HP
int Character::getHP() const {
    int result = currentHP;
    return result;
}

// Getter for max HP
int Character::getMaxHP() const {
    int result = maxHP;
    return result;
}

// Getter for character level
int Character::getLevel() const {
    int result = level;
    return result;
}

// Getter for experience points
int Character::getXP() const {
    int result = xp;
    return result;
}

// Getter for character class
string Character::getClass() const {
    string result = charClass;
    return result;
}

// Returns true if the character is alive
bool Character::isAlive() const {
    bool alive = currentHP > 0;
    return alive;
}

// Performs an attack based on the character's class
int Character::attack() const {
    int damage = 0;

    if (charClass == "Warrior") {
        damage = rollDice(12);
        cout << "Warrior swings a longsword! Deals " << damage << " damage." << endl;
    }
    else if (charClass == "Rogue") {
        damage = rollDice(10);
        cout << "Rogue strikes with a shortsword! Deals " << damage << " damage." << endl;
    }
    else if (charClass == "Cleric") {
        damage = rollDice(8);
        cout << "Cleric bashes with a club! Deals " << damage << " damage." << endl;
    }
    else if (charClass == "Wizard") {
        int missiles = 3 + (level - 1); // Base 3 missiles + 1 per level
        cout << "Wizard casts Magic Missile! (" << missiles << " missiles)" << endl;
        for (int i = 0; i < missiles; i++) {
            int missileDmg = rollDice(6) + 1;
            cout << "  ➤ Missile " << (i + 1) << " hits for " << missileDmg << " damage." << endl;
            damage += missileDmg;
        }
        cout << "Total Magic Missile damage: " << damage << endl;
    }

    if (buffed) {
        damage += 2;
        cout << "Energy buff adds +2 bonus damage!" << endl;
    }

    return damage;
}

// Reduces current HP by the specified amount
void Character::takeDamage(int dmg) {
    currentHP -= dmg;
    if (currentHP < 0) currentHP = 0;
}

// Adds experience points and handles level up
void Character::gainXP(int amount) {
    xp += amount;
    cout << "You gained " << amount << " XP";

    // Optionally show if it was earned from gold
    cout << " and collected " << amount << " e-Gold!" << endl;

    // Level up for every 200 XP
    while (xp >= 200) {
        xp -= 200;
        level++;

        // Roll additional HP based on class
        int extraHP = 0;
        if (charClass == "Warrior") extraHP = rollDice(12);
        else if (charClass == "Rogue") extraHP = rollDice(10);
        else if (charClass == "Cleric") extraHP = rollDice(8);
        else if (charClass == "Wizard") extraHP = rollDice(6);

        maxHP += extraHP;
        currentHP = maxHP;  // Fully heal on level up

        cout << "\n=== LEVEL UP! ===" << endl;
        cout << "You are now level " << level << "!" << endl;
        cout << "+ HP increased by " << extraHP << " (new max HP: " << maxHP << ")" << endl;
    }
}

// Restores character's state (used for loading)
void Character::restoreState(const string& playerClass, int lvl, int hp) {
    charClass = playerClass;
    level = lvl;
    currentHP = hp;
    calculateStats();
}

// Restores a percentage of missing HP
void Character::restoreHPPercent(int percent) {
    int missingHP = maxHP - currentHP;
    int healAmount = (missingHP * percent) / 100;
    currentHP += healAmount;
    if (currentHP > maxHP) currentHP = maxHP;
}

// Fully heal the character
void Character::healToFull() {
    currentHP = maxHP;
}

// Apply a temporary energy buff
void Character::applyEnergyBuff() {
    buffed = true;
    buffTurns = 10;
}

// Decrement energy buff duration and disable when expired
void Character::updateEnergyBuff() {
    if (buffed) {
        buffTurns--;
        if (buffTurns <= 0) {
            buffed = false;
            cout << "Your energy boost has worn off." << endl;
        }
    }
}

// Displays XP progress bar in console
void Character::showXPBar() const {
    int bars = (xp * 20) / 200;
    cout << "[";
    for (int i = 0; i < 20; i++) {
        if (i < bars) cout << "=";
        else cout << " ";
    }
    cout << "]" << endl;
}

// Returns dexterity stat
int Character::getDexterity() const {
    int result = dexterity;
    return result;
}

// Display character stats in a formatted manner
void Character::showStats() const {
    cout << "\n+========= CHARACTER STATS =========+" << endl;
    cout << "| Name:        " << name << endl;
    cout << "| Class:       " << charClass << endl;
    cout << "| Level:       " << level << endl;
    cout << "| HP:          " << currentHP << "/" << maxHP << endl;
    cout << "| Strength:    " << strength << endl;
    cout << "| Dexterity:   " << dexterity << endl;
    cout << "| Constitution:" << constitution << endl;
    cout << "| Intelligence:" << intelligence << endl;
    cout << "| Wisdom:      " << wisdom << endl;
    cout << "| Charisma:    " << charisma << endl;
    cout << "| XP:          " << xp << "/200" << endl;
    cout << "+===================================+\n" << endl;
    cout << "XP Progress: ";
    showXPBar();
}

// Returns a string representation of the HP bar
string Character::getHPBar() const {
    int totalBars = 20;
    int filledBars = (currentHP * totalBars) / maxHP;
    string bar = "[";
    for (int i = 0; i < totalBars; i++) {
        if (i < filledBars) bar += "=";
        else bar += " ";
    }
    bar += "] " + to_string(currentHP) + "/" + to_string(maxHP);
    return bar;
}

// Restore a more detailed set of character state values
void Character::restoreStateExtended(const string& n, const string& cls, int lvl, int hp, int str, int dex, int xpVal) {
    name = n;
    charClass = cls;
    level = lvl;
    currentHP = hp;
    strength = str;
    dexterity = dex;
    xp = xpVal;
    calculateStats();
}

// Get character name
string Character::getName() const {
    return name;
}

// Get strength value
int Character::getStrength() const {
    return strength;
}

// Getters for remaining attributes
int Character::getConstitution() const { return constitution; }
int Character::getIntelligence() const { return intelligence; }
int Character::getWisdom() const { return wisdom; }
int Character::getCharisma() const { return charisma; }

// Restore full character state with all attributes
void Character::restoreFullState(const string& n, const string& cls,
                                 int lvl, int hp, int maxHp,
                                 int str, int dex, int con,
                                 int intel, int wis, int cha,
                                 int xpVal) {
    name = n;
    charClass = cls;
    level = lvl;
    currentHP = hp;
    maxHP = maxHp;
    strength = str;
    dexterity = dex;
    constitution = con;
    intelligence = intel;
    wisdom = wis;
    charisma = cha;
    xp = xpVal;
}
