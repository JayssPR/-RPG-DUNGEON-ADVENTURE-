#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include <string>
using namespace std;

// Class representing the player's character
class Character {
private:
    string name;         // Character's name
    string charClass;    // Character's class
    int strength;        // Strength
    int dexterity;       // Dexterity
    int constitution;    // Constitution
    int intelligence;    // Intelligence
    int wisdom;          // Wisdom
    int charisma;        // Charisma
    int currentHP;       // Current HP
    int maxHP;           // Maximum HP
    int level;           // Current level
    int xp;              // Experience points
    bool buffed;         // Is under energy potion effect?
    int buffTurns;       // Remaining buff turns

    int calculateHP() const; // Calculates initial HP based on class

public:
    Character();                             // Constructor
    void createCharacter();                  // Creates a new character
    void calculateStats();                   // Calculates stats
    int getHP() const;                       // Returns current HP
    int getMaxHP() const;                    // Returns maximum HP
    int getLevel() const;                    // Returns current level
    int getXP() const;                       // Returns current XP
    string getClass() const;                 // Returns class
    bool isAlive() const;                    // Is still alive?
    int attack() const;                      // Damage per attack
    void takeDamage(int dmg);                // Takes damage
    void gainXP(int amount);                 // Gains XP and levels up
    void restoreState(const string& playerClass, int lvl, int hp); // Restores from save
    void restoreHPPercent(int percent);      // Restores % of missing HP
    void healToFull();                       // Fully restores HP
    void applyEnergyBuff();                  // Activates energy buff
    void updateEnergyBuff();                 // Decreases buff duration
    void showXPBar() const;                  // Displays XP bar
    int getDexterity() const;                // Returns dexterity
    void showStats() const;                  // Displays all stats
    string getHPBar() const;                 // Displays HP bar
    void showGeneratedStats() const;         // Shows generated attributes
    void showCreationSummary() const;        // Shows summary after creation
    void showLevelIntro(int level) const;    // Displays level intro
    string getName() const;                  // Returns character name
    int getStrength() const;                 // Returns strength

    void restoreStateExtended(const string& name, const string& className,
                               int lvl, int hp, int str, int dex, int xpVal); // Extended restore

    int getConstitution() const;             // Returns constitution
    int getIntelligence() const;             // Returns intelligence
    int getWisdom() const;                   // Returns wisdom
    int getCharisma() const;                 // Returns charisma

    void restoreFullState(const string& name, const string& className,
                          int lvl, int hp, int maxHp,
                          int str, int dex, int con,
                          int intel, int wis, int cha,
                          int xpVal);        // Fully restores character state
};

#endif // CHARACTER_H_INCLUDED

