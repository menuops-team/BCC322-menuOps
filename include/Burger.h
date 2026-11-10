
#ifndef BURGER_H
#define BURGER_H

#include <string>
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
    ~Burger();

public:
    // ---- Singleton access ----
    static Burger& createBurger();

    Burger(const Burger& other) = delete;
    Burger& operator=(const Burger& other) = delete;
    Burger(Burger&& other) = delete;
    Burger& operator=(Burger&& other) = delete;

    // Static factories assign IDs and store the created objects in the
    // canonical Burger instance returned by createBurger().
    static Resource& createResource(const std::string& name, const std::string& unit, double purchasePrice);
    static Supplier& createSupplier(const std::string& name, const std::string& cnpj,
        const std::string& contact, const std::string& deliveryTerms);
    static Shipment& createShipment(int supplierId, const Date& receiptDate);

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
