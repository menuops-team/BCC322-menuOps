
#ifndef BURGER_H
#define BURGER_H

#include <vector>
#include "Date.h"
#include "Resource.h"
#include "Supplier.h"
#include "Shipment.h"
#include "Batch.h"
#include "ShelfLifeTable.h"
#include "Inventory.h"

// Domain root: the only class responsible for creating and destroying
// resources, suppliers, and shipments. Inventory manages the batches.
class Burger {

    std::vector<Resource*> resourceList;
    std::vector<Supplier*> supplierList;
    std::vector<Shipment*> shipmentList;

    Inventory generalInventory;
    ShelfLifeTable shelfLifeTable;
    int nextId;

    void release();
    void swap(Burger& other) noexcept;

    explicit Burger();

public:
    // ---- Canonical form ----
    static Burger& createBurger();

    Burger(const Burger& other);              // Deep copy
    Burger& operator=(const Burger& other);   // Deep copy
    virtual ~Burger();

    // Creation: the root assigns IDs and owns the objects.
    Resource& createResource(const Resource& model);
    Supplier& createSupplier(const Supplier& model);
    Shipment& createShipment(const Shipment& model);

    // Search: returns nullptr if not found.
    // The caller must not delete the returned pointer.
    Resource* searchResource(int id) const;
    Supplier* searchSupplier(int id) const;
    Shipment* searchShipment(int id) const;

    // Read-only access to collections.
    const std::vector<Resource*>& getResources() const;
    const std::vector<Supplier*>& getSuppliers() const;
    const std::vector<Shipment*>& getShipments() const;

    // Subobjects owned by the domain root.
    Inventory& getInventory();
    const Inventory& getInventory() const;
    ShelfLifeTable& getShelfLifeTable();
    const ShelfLifeTable& getShelfLifeTable() const;
};

#endif
