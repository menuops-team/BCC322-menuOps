#include "../include/ResourceBatchService.h"
#include "../include/Burger.h"
#include "../include/Resource.h"
#include "../include/Supplier.h"
#include "../include/Shipment.h"

#include <algorithm>
#include <iterator>

namespace {

// Civil calendar <-> day count conversion (proleptic Gregorian).
long daysFromCivil(int y, int m, int d) {
    y -= m <= 2;
    const long era = (y >= 0 ? y : y - 399) / 400;
    const long yoe = y - era * 400;
    const long doy = (153L * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const long doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + doe - 719468;
}

Date civilFromDays(long z) {
    z += 719468;
    const long era = (z >= 0 ? z : z - 146096) / 146097;
    const long doe = z - era * 146097;
    const long yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    long y = yoe + era * 400;
    const long doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    const long mp = (5 * doy + 2) / 153;
    const int d = static_cast<int>(doy - (153 * mp + 2) / 5 + 1);
    const int m = static_cast<int>(mp < 10 ? mp + 3 : mp - 9);
    y += (m <= 2);
    return Date(d, m, static_cast<int>(y));
}

Date addDays(const Date& date, int days) {
    return civilFromDays(daysFromCivil(date.getYear(), date.getMonth(), date.getDay()) + days);
}

template <typename T>
int nextId(const std::vector<T*>& list) {
    int maxId = 0;
    typename std::vector<T*>::const_iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        if (*it != nullptr && (*it)->getId() > maxId) {
            maxId = (*it)->getId();
        }
    }
    return maxId + 1;
}

// A batch can still be consumed (not expired, not depleted).
bool isUsable(const Batch& b) {
    return (b.getStatus() == BatchStatus::AVAILABLE ||
            b.getStatus() == BatchStatus::NEAR_EXPIRATION) &&
           b.getQuantity() > 0.0;
}

// FEFO: first expired, first out. Ties are broken by id (oldest batch first).
bool consumeBefore(const Batch& a, const Batch& b) {
    if (a.getExpirationDate() < b.getExpirationDate()) 
        return true;
    if (b.getExpirationDate() < a.getExpirationDate()) 
        return false;
    return a.getId() < b.getId();
}

}

ResourceBatchService::~ResourceBatchService() {}

// Returns the new id, or -1 if the data is invalid.
int ResourceBatchService::createResource(const std::string& name, const std::string& unit,double purchasePrice) {
    if (name.empty() || unit.empty() || purchasePrice < 0.0) 
        return -1;

    // Rejects a duplicated name.
    const std::vector<Resource*>& resources = burger.getResources();
    std::vector<Resource*>::const_iterator it;
    for (it = resources.begin(); it != resources.end(); ++it) {
        if ((*it)->getName() == name) return -1;
    }

    const int id = nextId(resources);
    Resource* resource = new Resource(id, name, unit, purchasePrice);
    resource->setActive(true);
    burger.addResource(resource);
    return id;
}

// Returns the new id, or -1 if the data is invalid or the CNPJ is already registered.
int ResourceBatchService::createSupplier(const std::string& name, const std::string& cnpj, const std::string& contact, const std::string& deliveryTerms) {
    if (name.empty() || cnpj.empty()) 
        return -1;

    const std::vector<Supplier*>& suppliers = burger.getSuppliers();
    std::vector<Supplier*>::const_iterator it;
    for (it = suppliers.begin(); it != suppliers.end(); ++it) {
        if ((*it)->getCnpj() == cnpj)
            return -1;
    }

    const int id = nextId(suppliers);
    Supplier* supplier = new Supplier(id, name, cnpj, contact, deliveryTerms);
    supplier->setActive(true);
    burger.addSupplier(supplier);
    return id;
}

// Returns the new id, or -1 if the supplier does not exist/is inactive or the date is invalid.
int ResourceBatchService::createShipment(int supplierId, const Date& receiptDate) {
    const Supplier* supplier = burger.searchSupplier(supplierId);
    if (supplier == nullptr || !supplier->isActive()) return -1;
    if (!receiptDate.isValid()) return -1;

    const int id = nextId(burger.getShipments());
    Shipment* shipment = new Shipment(id, supplierId, receiptDate);
    shipment->setAccepted(false);
    burger.addShipment(shipment);
    return id;
}

// Registers a shipment built by the caller (resources already added to it).
// The service assigns the id (the one inside `shipment` is ignored) and keeps its own copy.
// Returns the new id, or -1 if the supplier does not exist/is inactive, the date is invalid,
// or any item refers to an unknown resource or has a non-positive quantity.
int ResourceBatchService::createShipment(const Shipment& shipment) {
    const Supplier* supplier = burger.searchSupplier(shipment.getSupplierId());
    if (supplier == nullptr || !supplier->isActive()) 
        return -1;
    if (!shipment.getReceiptDate().isValid()) 
        return -1;

    const std::vector<int>& resourceIds = shipment.getResourceIds();
    const std::vector<double>& quantities = shipment.getQuantities();
    if (resourceIds.size() != quantities.size()) 
        return -1;

    std::vector<int>::const_iterator rit = resourceIds.begin();
    std::vector<double>::const_iterator qit = quantities.begin();
    for (; rit != resourceIds.end() && qit != quantities.end(); ++rit, ++qit) {
        if (burger.searchResource(*rit) == nullptr || *qit <= 0.0) 
            return -1;
    }

    Shipment* copy = new Shipment(shipment);
    const int id = nextId(burger.getShipments());
    copy->setId(id);
    burger.addShipment(copy);
    return id;
}

// Opens one batch per shipment item: expiration = receipt date + shelf life of the resource.
// All-or-nothing: if any item is invalid, nothing is created and an empty vector is returned.
// The shipment must have passed the inspection (accepted) and can only be opened once.
std::vector<int> ResourceBatchService::openBatches(int shipmentId) {
    std::vector<int> ids;

    Shipment* shipment = burger.searchShipment(shipmentId);
    if (shipment == nullptr || !shipment->isAccepted()) 
        return ids;

    const std::vector<Batch>& existing = burger.getInventory().getBatches();
    std::vector<Batch>::const_iterator bit;
    for (bit = existing.begin(); bit != existing.end(); ++bit) {
        if (bit->getShipmentId() == shipmentId) return ids;
    }

    const std::vector<int>& resourceIds = shipment->getResourceIds();
    const std::vector<double>& quantities = shipment->getQuantities();
    if (resourceIds.empty() || resourceIds.size() != quantities.size()) 
        return ids;

    const Date receipt = shipment->getReceiptDate();
    const ShelfLifeTable& table = burger.getShelfLifeTable();

    //validate everything before changing any state.
    std::vector<int>::const_iterator rit = resourceIds.begin();
    std::vector<double>::const_iterator qit = quantities.begin();
    for (; rit != resourceIds.end() && qit != quantities.end(); ++rit, ++qit) {
        const Resource* resource = burger.searchResource(*rit);
        if (resource == nullptr || !resource->isActive()) 
            return ids;
        if (*qit <= 0.0) 
            return ids;
        if (!table.containsResource(*rit) || table.getShelfLife(*rit) <= 0) 
            return ids;
    }

    //create the batches.
    Inventory& inventory = burger.getInventory();
    rit = resourceIds.begin();
    qit = quantities.begin();
    for (; rit != resourceIds.end() && qit != quantities.end(); ++rit, ++qit) {
        const Resource* resource = burger.searchResource(*rit);

        Batch batch;
        batch.setId(nextBatchId++);
        batch.setResourceId(*rit);
        batch.setShipmentId(shipmentId);
        batch.setQuantity(*qit);
        batch.setPurchasePrice(resource->getPurchasePrice());
        batch.setExpirationDate(addDays(receipt, table.getShelfLife(*rit)));
        batch.setStatus(BatchStatus::AVAILABLE);

        inventory.addBatch(batch);
        ids.push_back(batch.getId());
    }

    return ids;
}

// Batches that can still be used, ordered by expiration date (FEFO).
std::vector<Batch> ResourceBatchService::getPriorityBatches() const {
    const std::vector<Batch>& all = burger.getInventory().getBatches();

    std::vector<Batch> result;
    std::back_insert_iterator<std::vector<Batch> > out(result);
    std::vector<Batch>::const_iterator it;
    for (it = all.begin(); it != all.end(); ++it) {
        if (isUsable(*it)) {
            *out++ = *it;
        }
    }

    std::sort(result.begin(), result.end(), consumeBefore);
    return result;
}