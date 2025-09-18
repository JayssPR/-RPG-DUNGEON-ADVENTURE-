#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

#include <string>
using namespace std;

// Rolls a die with a given number of sides (uses srand and time)
int rollDice(int sides);

// Generates a character stat by rolling 4d6 and summing the top 3
int rollStat();

// Validates an integer input within a specified range
int getValidatedInput(int min, int max);

// Validates a character input against a set of valid options
char getValidatedCharInput(const string& validOptions);

// Displays the game's main menu
void showMainMenu();

// Returns true if a roll was 20 (critical hit)
bool isCriticalHit(int roll);

// Generates a stat and labels it with the given stat name
int rollStat(const string& statName);

#endif // UTILITIES_H_INCLUDED
