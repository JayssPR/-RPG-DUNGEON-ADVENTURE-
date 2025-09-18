#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <string>
using namespace std;

// Types of items available in the game
enum ItemType {
    FOOD,
    HEALING_POTION,
    ENERGY_POTION,
    EGOLD
};

// Class that represents an in-game item
class Item {
private:
    string name;      // Item's name
    ItemType type;    // Item's type

public:
    Item();                              // Default constructor
    Item(const string& n, ItemType t);   // Parameterized constructor
    string getName() const;              // Returns the item's name
    ItemType getType() const;            // Returns the item's type
};

#endif // ITEM_H_INCLUDED
