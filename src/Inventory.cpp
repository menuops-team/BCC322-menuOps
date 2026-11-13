#include "Inventory.h"

using namespace std;

Inventory::Inventory(const vector<Batch>& batches): batches(batches) {}

Inventory::Inventory(const Inventory& other): batches(other.batches) {}

Inventory& Inventory::operator=(const Inventory& other) {
    if (this != &other) {
        batches = other.batches;
    }
    return *this;
}

void Inventory::addBatch(const Batch& batch) {
    batches.push_back(batch);
}

bool Inventory::removeBatch(int index) {
    if (index < 0 || index >= (int) batches.size()) {
        return false;
    }
    batches.erase(batches.begin() + index);
    return true;
}

vector<Batch>& Inventory::getBatches() {
    return batches;
}

const vector<Batch>& Inventory::getBatches() const {
    return batches;
}

int Inventory::getBatchCount() const {
    return batches.size();
}

bool Inventory::isEmpty() const {
    return batches.empty();
}

void Inventory::clear() {
    batches.clear();
}
