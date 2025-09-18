#include "inventory.h"
#include "utilities.h"
#include <iostream>

using namespace std;

// Constructor initializes item count and gold to zero
Inventory::Inventory() {
    itemCount = 0;
    gold = 0;
}

// Adds an item to the inventory or awards gold/XP if the item is e-Gold
void Inventory::addItem(const Item& item, Character& player) {
    if (item.getType() == EGOLD) {
        int amount = (rand() % 20) + 1; // Random gold between 1 and 20
        addGold(amount);                // Add gold to inventory
        player.gainXP(amount);          // Player gains XP equal to gold
        cout << "You found e-Gold! +" << amount << " XP gained." << endl;
    } else if (itemCount < 10) {        // Limit of 10 items in inventory
        items[itemCount] = item;
        itemCount++;
    }
}

// Uses an item by index, applying its effect to the player
void Inventory::useItem(int index, Character& player) {
    bool valid = true;
    bool used = false;

    if (index < 1 || index > itemCount) {
        cout << "Invalid item number." << endl;
        valid = false;
    }

    if (valid) {
        Item& item = items[index - 1];

        cout << "Are you sure you want to use " << item.getName() << "? (Y/N): ";
        char confirm = getValidatedCharInput("Y/N");

        if (confirm == 'Y') {
            if (item.getType() == FOOD) {
                player.restoreHPPercent(20);
                cout << "You eat the food and restore 20% of missing HP." << endl;
                used = true;
            } else if (item.getType() == HEALING_POTION) {
                player.healToFull();
                cout << "You drink the healing potion and fully restore your HP!" << endl;
                used = true;
            } else if (item.getType() == ENERGY_POTION) {
                player.applyEnergyBuff();
                cout << "You drink the energy potion. Strength boosted for 10 turns!" << endl;
                used = true;
            }
        } else {
            cout << "Cancelled using the item." << endl;
        }

        // If item was used, remove it from inventory
        if (used) {
            for (int i = index - 1; i < itemCount - 1; i++) {
                items[i] = items[i + 1];
            }
            itemCount--;
        }
    }

    return;
}


// Displays the current inventory with items and descriptions
void Inventory::showInventory() const {
    cout << "\n+========== INVENTORY ==========+" << endl;
    if (itemCount == 0) {
        cout << "| You have no items.            |" << endl;
    } else {
        for (int i = 0; i < itemCount; i++) {
            string name = items[i].getName();
            string desc = getItemDescription(items[i]);
            cout << "| [" << (i + 1) << "] " << name;
            int spacing = 20 - name.length();
            for (int s = 0; s < spacing; s++) cout << " ";
            cout << "- " << desc << " |\n";
        }
    }

    // Display current e-Gold
    cout << "| e-Gold: " << gold;
    int goldSpace = 26 - to_string(gold).length();
    for (int s = 0; s < goldSpace; s++) {
        cout << " ";
    }
    cout << "|\n";
    cout << "+===============================+\n" << endl;
}

// Returns the amount of e-Gold in inventory
int Inventory::getGold() const {
    int result = gold;
    return result;
}

// Adds a specified amount of gold to the inventory
void Inventory::addGold(int amount) {
    gold += amount;
}

// Returns the current number of items
int Inventory::getItemCount() const {
    int result = itemCount;
    return result;
}

// Returns the item at the specified index
Item Inventory::getItem(int index) const {
    Item result = Item();
    if (index >= 0 && index < itemCount) {
        result = items[index];
    }
    return result;
}

// Adds an item to the inventory without triggering e-Gold XP logic
void Inventory::addItemDirectly(const Item& item) {
    if (itemCount < 10) {
        items[itemCount++] = item;
    }
}

// Returns a string description of the item based on its type
string Inventory::getItemDescription(const Item& item) const {
    string description = "Unknown item.";
    if (item.getType() == FOOD) {
        description = "Restores 20% of missing HP.";
    } else if (item.getType() == HEALING_POTION) {
        description = "Fully restores HP.";
    } else if (item.getType() == ENERGY_POTION) {
        description = "Boosts Strength by +2 for 10 rounds.";
    } else if (item.getType() == EGOLD) {
        description = "Collects e-Gold (adds XP).";
    }
    return description;
}

// Removes an item from inventory and returns it by reference
bool Inventory::dropItem(int index, Item& droppedItem) {
    bool success = false;

    if (index >= 1) {
        if (index <= itemCount) {
            droppedItem = items[index - 1];

            for (int i = index - 1; i < itemCount - 1; i++) {
                items[i] = items[i + 1];
            }

            itemCount--;
            success = true;
        }
    }

    return success;
}

