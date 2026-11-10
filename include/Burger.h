
#ifndef BURGER_H
#define BURGER_H

#include <vector>
#include "ShelfLifeTable.h"
#include "Inventory.h"

class Resource;
class Supplier;
class Shipment;

// Domain root: stores resources, suppliers, shipments, and inventory state.
class Burger {

    std::vector<Resource*> resourceList;
    std::vector<Supplier*> supplierList;
    std::vector<Shipment*> shipmentList;

    Inventory generalInventory;
    ShelfLifeTable shelfLifeTable;

public:
    Burger();
    ~Burger();

    Burger(const Burger& other);
    Burger& operator=(const Burger& other);
    Burger(Burger&& other);
    Burger& operator=(Burger&& other);

    // Add operations store the objects in this Burger.
    void addResource(Resource* resource);
    void addSupplier(Supplier* supplier);
    void addShipment(Shipment* shipment);

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
