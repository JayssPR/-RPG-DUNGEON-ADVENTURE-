#ifndef INVENTORY_H_INCLUDED
#define INVENTORY_H_INCLUDED

#include "item.h"
#include "character.h"
#include <string>
using namespace std;

// Class that manages the player's items and e-Gold
class Inventory {
private:
    Item items[10];     // List of up to 10 items
    int itemCount;      // Current number of items
    int gold;           // Amount of e-Gold

    string getItemDescription(const Item& item) const; // Item description

public:
    Inventory();

    // Adds an item or e-Gold to the inventory and applies effects
    void addItem(const Item& item, Character& player);

    // Uses an item from the inventory
    void useItem(int index, Character& player);

    // Displays inventory contents
    void showInventory() const;

    // Gets the current gold amount
    int getGold() const;

    // Adds a specific amount of gold
    void addGold(int amount);

    // Returns the number of current items
    int getItemCount() const;

    // Accesses an individual item
    Item getItem(int index) const;

    // Adds an item directly without applying effects
    void addItemDirectly(const Item& item);

    // Drops an item from the inventory
    bool dropItem(int index, Item& droppedItem);
};

#endif // INVENTORY_H_INCLUDED
