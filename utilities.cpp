#include "utilities.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;


// Returns a random number between 1 and "sides".
int rollDice(int sides) {
    return (rand() % sides) + 1;
}

// Rolls 4d6 and returns the sum of the best 3 results.
// This is commonly used for generating RPG character stats.
int rollStat() {
    int rolls[4];
    // Roll four 6-sided dice
    for (int i = 0; i < 4; i++) {
        rolls[i] = rollDice(6);
    }

    // Sort the dice rolls using bubble sort (ascending)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3 - i; j++) {
            if (rolls[j] > rolls[j + 1]) {
                int temp = rolls[j];
                rolls[j] = rolls[j + 1];
                rolls[j + 1] = temp;
            }
        }
    }

    // Return the sum of the top 3 rolls
    int result = rolls[1] + rolls[2] + rolls[3];
    return result;
}

// Reads and validates an integer input between [min, max].
// Ensures only numeric input and that the number is within the specified range.
int getValidatedInput(int min, int max) {
    int input = min - 1;
    bool valid = false;

    while (!valid) {
        string line;
        getline(cin, line);

        bool isValid = true;
        bool negative = false;
        int number = 0;

        if (!line.empty()) {
            int i = 0;

            // Check for negative sign
            if (line[0] == '-') {
                negative = true;
                i = 1;
                if (line.length() == 1) isValid = false;
            }

            // Ensure all characters are digits
            while (i < (int)line.length()) {
                if (line[i] < '0' || line[i] > '9') {
                    isValid = false;
                }
                i++;
            }

            // Convert string to integer if valid
            if (isValid) {
                if (negative) {
                    i = 1;
                } else {
                    i = 0;
                }

                while (i < (int)line.length()) {
                    number = number * 10 + (line[i] - '0');
                    i++;
                }

                if (negative) number = -number;

                if (number >= min && number <= max) {
                    input = number;
                    valid = true;
                } else {
                    cout << "Number out of range (" << min << "-" << max << "). Try again: ";
                }
            } else {
                cout << "Invalid input. Please enter a number: ";
            }
        } else {
            cout << "Invalid input. Please enter a number: ";
        }
    }

    return input;
}



// Reads and validates a single character input.
// Accepts only characters found in the "validOptions" string.
char getValidatedCharInput(const string& validOptions) {
    char input = '\0';
    bool valid = false;

    while (!valid) {
        string line;
        getline(cin, line);

        if (!line.empty()) {
            input = line[0];

            // Convert to uppercase if lowercase
            if (input >= 'a' && input <= 'z') {
                input = input - 'a' + 'A';
            }

            // Check if input matches any valid option (case insensitive)
            for (size_t i = 0; i < validOptions.length(); ++i) {
                char option = validOptions[i];
                if (option >= 'a' && option <= 'z') {
                    option = option - 'a' + 'A';
                }

                if (input == option) {
                    valid = true;
                }
            }

            if (!valid) {
                cout << "Invalid choice. Valid options are: " << validOptions << ". Try again: ";
            }
        } else {
            cout << "Please enter a letter: ";
        }
    }

    return input;
}

// Displays the main menu of the game
void showMainMenu() {
    cout << "\n+========================================+" << endl;
    cout << "|         RPG DUNGEON ADVENTURE          |" << endl;
    cout << "+========================================+" << endl;
    cout << "| [1] New Game                           |" << endl;
    cout << "| [2] Load Game                          |" << endl;
    cout << "| [3] Exit                               |" << endl;
    cout << "+========================================+" << endl;
    cout << "Choose an option: ";
}

// Checks if a given dice roll is a natural 20 (critical hit)
bool isCriticalHit(int roll) {
    bool result = (roll == 20);
    return result;
}
