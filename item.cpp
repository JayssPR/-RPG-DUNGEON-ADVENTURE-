#include "item.h"

// Default constructor initializes an "Empty" item of type FOOD
Item::Item() {
    name = "Empty";
    type = FOOD;
}

// Constructor with parameters sets the item name and type
Item::Item(const string& n, ItemType t) {
    name = n;
    type = t;
}

// Returns the name of the item
string Item::getName() const {
    string result = name;
    return result;
}

// Returns the type of the item (enum ItemType)
ItemType Item::getType() const {
    ItemType result = type;
    return result;
}
