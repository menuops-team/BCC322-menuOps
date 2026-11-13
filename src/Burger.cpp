#include "Burger.h"
#include "Resource.h"
#include "Supplier.h"
#include "Shipment.h"
#include <utility>
using namespace std;

namespace{
    
template <typename T>
void addObject(vector<T*>& container, T* object) {
    if (object != nullptr) {
        container.push_back(object);
    }
}

template <typename T>
T* searchObject(const vector<T*>& container, int id) {
    typename vector<T*>::const_iterator it;

    for (it = container.begin(); it != container.end(); ++it) {
        if ((*it)->getId() == id) {
            return *it;
        }
    }

    return nullptr;
}

}

Burger::Burger() : generalInventory(vector<Batch>()) {}

Burger::~Burger() {
    vector<Resource*>::iterator resourceIt;
    for (resourceIt = resourceList.begin(); resourceIt != resourceList.end(); ++resourceIt) {
        delete *resourceIt;
    }

    vector<Supplier*>::iterator supplierIt;
    for (supplierIt = supplierList.begin(); supplierIt != supplierList.end(); ++supplierIt) {
        delete *supplierIt;
    }

    vector<Shipment*>::iterator shipmentIt;
    for (shipmentIt = shipmentList.begin(); shipmentIt != shipmentList.end(); ++shipmentIt) {
        delete *shipmentIt;
    }
}

Burger::Burger(const Burger& other) : generalInventory(other.generalInventory), shelfLifeTable(other.shelfLifeTable) {

    try {
        vector<Resource*>::const_iterator resourceIt;
        for (resourceIt = other.resourceList.begin();
             resourceIt != other.resourceList.end(); ++resourceIt) {
            resourceList.push_back(new Resource(**resourceIt));
        }

        vector<Supplier*>::const_iterator supplierIt;
        for (supplierIt = other.supplierList.begin();
             supplierIt != other.supplierList.end(); ++supplierIt) {
            supplierList.push_back(new Supplier(**supplierIt));
        }

        vector<Shipment*>::const_iterator shipmentIt;
        for (shipmentIt = other.shipmentList.begin();
             shipmentIt != other.shipmentList.end(); ++shipmentIt) {
            shipmentList.push_back(new Shipment(**shipmentIt));
        }
    }
    catch (...) {
        vector<Resource*>::iterator resourceIt;
        for (resourceIt = resourceList.begin(); resourceIt != resourceList.end(); ++resourceIt) {
            delete *resourceIt;
        }

        vector<Supplier*>::iterator supplierIt;
        for (supplierIt = supplierList.begin(); supplierIt != supplierList.end(); ++supplierIt) {
            delete *supplierIt;
        }

        vector<Shipment*>::iterator shipmentIt;
        for (shipmentIt = shipmentList.begin(); shipmentIt != shipmentList.end(); ++shipmentIt) {
            delete *shipmentIt;
        }

        throw;
    }
}

Burger& Burger::operator=(const Burger& other) {
    if (this != &other) {
        Burger temp(other);
        *this = move(temp);
    }

    return *this;
}

Burger::Burger(Burger&& other)
    : resourceList(move(other.resourceList)),
      supplierList(move(other.supplierList)),
      shipmentList(move(other.shipmentList)),
      generalInventory(move(other.generalInventory)),
      shelfLifeTable(move(other.shelfLifeTable)) {

    other.resourceList.clear();
    other.supplierList.clear();
    other.shipmentList.clear();
}

Burger& Burger::operator=(Burger&& other) {
    if (this != &other) {
        vector<Resource*>::iterator resourceIt;
        for (resourceIt = resourceList.begin(); resourceIt != resourceList.end(); ++resourceIt) {
            delete *resourceIt;
        }

        vector<Supplier*>::iterator supplierIt;
        for (supplierIt = supplierList.begin(); supplierIt != supplierList.end(); ++supplierIt) {
            delete *supplierIt;
        }

        vector<Shipment*>::iterator shipmentIt;
        for (shipmentIt = shipmentList.begin(); shipmentIt != shipmentList.end(); ++shipmentIt) {
            delete *shipmentIt;
        }

        resourceList = move(other.resourceList);
        supplierList = move(other.supplierList);
        shipmentList = move(other.shipmentList);

        generalInventory = move(other.generalInventory);
        shelfLifeTable = move(other.shelfLifeTable);

        other.resourceList.clear();
        other.supplierList.clear();
        other.shipmentList.clear();
    }

    return *this;
}

//add 

void Burger::addResource(Resource* resource) {
    addObject(resourceList, resource);
}

void Burger::addSupplier(Supplier* supplier) {
    addObject(supplierList, supplier);
}

void Burger::addShipment(Shipment* shipment) {
    addObject(shipmentList, shipment);
}


//Search id
Resource* Burger::searchResource(int id) const {
    return searchObject(resourceList, id);
}

Supplier* Burger::searchSupplier(int id) const {
    return searchObject(supplierList, id);
}

Shipment* Burger::searchShipment(int id) const {
    return searchObject(shipmentList, id);
}


//Getters
const vector<Resource*>& Burger::getResources() const {
    return resourceList;
}

const vector<Supplier*>& Burger::getSuppliers() const {
    return supplierList;
}

const vector<Shipment*>& Burger::getShipments() const {
    return shipmentList;
}

Inventory& Burger::getInventory() {
    return generalInventory;
}

const Inventory& Burger::getInventory() const {
    return generalInventory;
}

ShelfLifeTable& Burger::getShelfLifeTable() {
    return shelfLifeTable;
}

const ShelfLifeTable& Burger::getShelfLifeTable() const {
    return shelfLifeTable;
}