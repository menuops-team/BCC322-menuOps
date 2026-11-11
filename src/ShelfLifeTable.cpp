#include "../include/ShelfLifeTable.h"
using namespace std;

ShelfLifeTable::ShelfLifeTable() : shelfLifeDays() {}

ShelfLifeTable::ShelfLifeTable(const ShelfLifeTable& other) : shelfLifeDays(other.shelfLifeDays) {}

ShelfLifeTable& ShelfLifeTable::operator=(const ShelfLifeTable& other) {
    if (this != &other) {
        shelfLifeDays = other.shelfLifeDays;
    }
    return *this;
}

void ShelfLifeTable::setShelfLife(int resourceId, int days) {
    shelfLifeDays[resourceId] = days;
}

int ShelfLifeTable::getShelfLife(int resourceId) const {
    if (shelfLifeDays.find(resourceId) != shelfLifeDays.end()) {
        return shelfLifeDays.at(resourceId);
    }
    return -1;
}

bool ShelfLifeTable::containsResource(int resourceId) const {
    return shelfLifeDays.find(resourceId) != shelfLifeDays.end();
}

bool ShelfLifeTable::removeResource(int resourceId) {
    return shelfLifeDays.erase(resourceId) > 0;
}

void ShelfLifeTable::clear() {
    shelfLifeDays.clear();
}