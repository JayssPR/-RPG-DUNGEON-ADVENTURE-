#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include <string>
using namespace std;

// Class representing an enemy
class Enemy {
private:
    string type;        // Enemy's class/type
    int level;          // Enemy's level
    int maxHP;          // Maximum HP
    int currentHP;      // Current HP
    int dexterity;      // Dexterity
    int strength;       // Strength

public:
    Enemy(const string& charType, int playerLevel); // Constructor with type and level

    int getHP() const;               // Returns current HP
    int getLevel() const;            // Returns level
    string getType() const;          // Returns type
    void takeDamage(int dmg);        // Takes damage
    int attack() const;              // Returns attack damage
    bool isAlive() const;            // Is alive?
    int getMaxHP() const;            // Returns maximum HP
    int getDexterity() const;        // Returns dexterity
    string getHPBar() const;         // Returns HP bar as a string
};

#endif // ENEMY_H_INCLUDED

