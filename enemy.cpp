#include "enemy.h"
#include "utilities.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constructor: initializes an enemy based on type and player level
Enemy::Enemy(const string& charType, int playerLevel) {
    unsigned seed = time(0);
    srand(seed);

    type = charType;

    // Level is player's level ±1, but never below 1
    level = playerLevel + (rand() % 3 - 1); // -1, 0, or +1
    if (level < 1) level = 1;

    // Determine base die for HP based on enemy type
    int baseDie = 6;
    if (type == "Warrior") baseDie = 12;
    else if (type == "Rogue") baseDie = 10;
    else if (type == "Cleric") baseDie = 8;
    else if (type == "Wizard") baseDie = 6;

    // Roll HP based on base die
    maxHP = (rand() % baseDie + 1) + baseDie;
    currentHP = maxHP;

    // Roll basic stats
    dexterity = rollStat();
    strength = rollStat();
}

// Returns current HP
int Enemy::getHP() const {
    int result = currentHP;
    return result;
}

// Returns enemy level
int Enemy::getLevel() const {
    int result = level;
    return result;
}

// Returns type/class of the enemy (e.g., "Rogue")
string Enemy::getType() const {
    string result = type;
    return result;
}

// Reduces current HP by damage taken, prevents going below 0
void Enemy::takeDamage(int dmg) {
    currentHP -= dmg;
    if (currentHP < 0) currentHP = 0;
}

// Returns attack damage based on enemy type
int Enemy::attack() const {
    int result = 0;
    if (type == "Warrior") result = (rand() % 12) + 1;
    else if (type == "Rogue") result = (rand() % 10) + 1;
    else if (type == "Cleric") result = (rand() % 8) + 1;
    else if (type == "Wizard") result = (rand() % 6) + 1;
    return result;
}

// Returns true if the enemy is still alive
bool Enemy::isAlive() const {
    bool alive = currentHP > 0;
    return alive;
}

// Returns the enemy's maximum HP
int Enemy::getMaxHP() const {
    int result = maxHP;
    return result;
}

// Returns enemy dexterity stat
int Enemy::getDexterity() const {
    int result = dexterity;
    return result;
}


// Returns a visual HP bar representation for the console
string Enemy::getHPBar() const {
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
