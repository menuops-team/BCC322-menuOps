#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include "Batch.h"

class Inventory {
private:
    std::vector<Batch> batches;

public:
    Inventory() = delete;
    explicit Inventory(const std::vector<Batch>& batches);
    Inventory(const Inventory& other);
    Inventory& operator=(const Inventory& other);
    virtual ~Inventory() = default;

    void addBatch(const Batch& batch);
    bool removeBatch(int index);

    std::vector<Batch>& getBatches();
    const std::vector<Batch>& getBatches() const;
    int getBatchCount() const;
    bool isEmpty() const;

    void clear();
};

#endif
