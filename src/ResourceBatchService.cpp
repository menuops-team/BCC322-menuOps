#include "ResourceBatchService.h"
#include "Burger.h"
#include "Resource.h"
#include "Supplier.h"
#include "Shipment.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iterator>
#include <map>

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

// A batch is "near expiration" when it expires within this many days (counting from today).
const int NEAR_EXPIRATION_DAYS = 3;

// Resource no longer stores a price and Shipment only carries quantities, so there is
// no purchase price to copy into a new batch yet.
const double UNKNOWN_PURCHASE_PRICE = 0.0;

// Largest difference (same unit as the quantity) still treated as "the same weight".
const double QUANTITY_TOLERANCE = 1e-6;

// Number of days from `from` to `to` (negative if `to` is before `from`).
long daysBetween(const Date& from, const Date& to) {
    return daysFromCivil(to.getYear(), to.getMonth(), to.getDay()) -
           daysFromCivil(from.getYear(), from.getMonth(), from.getDay());
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
    return (b.getStatus() == BatchStatus::AVAILABLE || b.getStatus() == BatchStatus::NEAR_EXPIRATION) && b.getQuantity() > 0.0;
}

// FEFO: first expired, first out. Ties are broken by id (oldest batch first).
bool consumeBefore(const Batch& a, const Batch& b) {
    if (a.getExpirationDate() < b.getExpirationDate()) return true;
    if (b.getExpirationDate() < a.getExpirationDate()) return false;
    return a.getId() < b.getId();
}

} 

ResourceBatchService::~ResourceBatchService() {}

// Returns the new id. Throws std::runtime_error if the name is empty/blank or already exists.
int ResourceBatchService::createResource(const std::string& name) {
    if (name.find_first_not_of(" \t\n\r\f\v") == std::string::npos) {
        throw std::runtime_error("Invalid resource data.");
    }

    // Rejects a duplicated name.
    const std::vector<Resource*>& resources = burger.getResources();
    std::vector<Resource*>::const_iterator it;
    for (it = resources.begin(); it != resources.end(); ++it) {
        if ((*it)->getName() == name) {
            throw std::runtime_error("Resource already exists.");
        }
    }

    const int id = nextId(resources);
    burger.addResource(new Resource(id, name));
    return id;
}

// Returns the new id. Throws std::runtime_error if the data is invalid or the CNPJ is already registered.
int ResourceBatchService::createSupplier(const std::string& name, const std::string& cnpj,
                                         const std::string& contact,
                                         const std::string& deliveryTerms) {
    if (name.empty() || cnpj.empty()) {
        throw std::runtime_error("Invalid supplier data.");
    }

    const std::vector<Supplier*>& suppliers = burger.getSuppliers();
    std::vector<Supplier*>::const_iterator it;
    for (it = suppliers.begin(); it != suppliers.end(); ++it) {
        if ((*it)->getCnpj() == cnpj) {
            throw std::runtime_error("Supplier already exists.");
        }
    }

    const int id = nextId(suppliers);
    Supplier* supplier = new Supplier(id, name, cnpj, contact, deliveryTerms);
    supplier->setActive(true);
    burger.addSupplier(supplier);
    return id;
}

// Returns the new id. Throws std::runtime_error if the supplier does not exist/is inactive or the date is invalid.
int ResourceBatchService::createShipment(int supplierId, const Date& receiptDate) {
    const Supplier* supplier = burger.searchSupplier(supplierId);
    if (supplier == nullptr) 
        throw std::runtime_error("Supplier not found.");
    if (!supplier->isActive()) 
        throw std::runtime_error("Supplier is inactive.");
    if (!receiptDate.isValid()) 
        throw std::runtime_error("Invalid receipt date.");

    const int id = nextId(burger.getShipments());
    Shipment* shipment = new Shipment(id, supplierId, receiptDate);
    shipment->setAccepted(false);
    burger.addShipment(shipment);
    return id;
}

// Registers a shipment built by the caller (resources already added to it).
// The service assigns the id (the one inside `shipment` is ignored) and keeps its own copy.
// Returns the new id. Throws std::runtime_error if the supplier does not exist/is inactive,
// the date is invalid, or any item refers to an unknown resource or has a non-positive quantity.
// Nothing is stored when it throws.
int ResourceBatchService::createShipment(const Shipment& shipment) {
    const Supplier* supplier = burger.searchSupplier(shipment.getSupplierId());
    if (supplier == nullptr) 
        throw std::runtime_error("Supplier not found.");
    if (!supplier->isActive()) 
        throw std::runtime_error("Supplier is inactive.");
    if (!shipment.getReceiptDate().isValid()) 
        throw std::runtime_error("Invalid receipt date.");

    const std::map<int, double>& items = shipment.getResources();
    std::map<int, double>::const_iterator it;
    for (it = items.begin(); it != items.end(); ++it) {
        if (burger.searchResource(it->first) == nullptr) {
            throw std::runtime_error("Resource not found.");
        }
        if (it->second <= 0.0) {
            throw std::runtime_error("Invalid shipment items.");
        }
    }

    Shipment* copy = new Shipment(shipment);
    const int id = nextId(burger.getShipments());
    copy->setId(id);
    burger.addShipment(copy);
    return id;
}

// Inspection of a received shipment: every measured quantity must match the declared one.
bool ResourceBatchService::acceptShipment(int shipmentId,
                                          const std::vector<double>& measuredQuantities) {
    Shipment* shipment = burger.searchShipment(shipmentId);
    if (shipment == nullptr || shipment->isAccepted()) return false;

    // The map is ordered by resource id, so measuredQuantities follows that same order.
    const std::map<int, double>& declared = shipment->getResources();
    if (declared.empty() || declared.size() != measuredQuantities.size()) return false;

    std::map<int, double>::const_iterator dit = declared.begin();
    std::vector<double>::const_iterator mit = measuredQuantities.begin();
    for (; dit != declared.end() && mit != measuredQuantities.end(); ++dit, ++mit) {
        if (std::fabs(dit->second - *mit) > QUANTITY_TOLERANCE) return false;
    }

    shipment->setAccepted(true);
    return true;
}

// Opens one batch per shipment item: expiration = receipt date + shelf life of the resource.
// All-or-nothing: if any item is invalid, nothing is created and an empty vector is returned.
// The shipment must have passed the inspection (acceptShipment), otherwise std::runtime_error is
// thrown. It can only be opened once.
std::vector<int> ResourceBatchService::openBatches(int shipmentId) {
    std::vector<int> ids;

    Shipment* shipment = burger.searchShipment(shipmentId);
    if (shipment == nullptr) return ids;
    if (!shipment->isAccepted()) {
        throw std::runtime_error("Shipment has not been accepted.");
    }

    const std::vector<Batch>& existing = burger.getInventory().getBatches();
    std::vector<Batch>::const_iterator bit;
    for (bit = existing.begin(); bit != existing.end(); ++bit) {
        if (bit->getShipmentId() == shipmentId) return ids;
    }

    const std::map<int, double>& items = shipment->getResources();
    if (items.empty()) return ids;

    const Date receipt = shipment->getReceiptDate();
    const ShelfLifeTable& table = burger.getShelfLifeTable();

    // validates everything before changing any state.
    std::map<int, double>::const_iterator it;
    for (it = items.begin(); it != items.end(); ++it) {
        const Resource* resource = burger.searchResource(it->first);
        if (resource == nullptr) return ids;
        if (it->second <= 0.0) return ids;
        if (!table.containsResource(it->first) || table.getShelfLife(it->first) <= 0) return ids;
    }

    // builds the batches (Batch has no default constructor). Nothing is stored
    // until every batch was built, so a failure here leaves the inventory untouched.
    std::vector<Batch> created;
    int batchId = nextBatchId;
    for (it = items.begin(); it != items.end(); ++it) {
        created.push_back(Batch(batchId++, it->first, shipmentId, it->second,
                                UNKNOWN_PURCHASE_PRICE,
                                addDays(receipt, table.getShelfLife(it->first)),
                                BatchStatus::AVAILABLE));
    }

    Inventory& inventory = burger.getInventory();
    std::vector<Batch>::const_iterator cit;
    for (cit = created.begin(); cit != created.end(); ++cit) {
        inventory.addBatch(*cit);
        ids.push_back(cit->getId());
    }
    nextBatchId = batchId;

    return ids;
}

// Recomputes the status of every batch for the given date:
//   no quantity left                       -> DEPLETED
//   expiration date before `today`          -> EXPIRED (a batch is still valid on its expiration date)
//   expires within NEAR_EXPIRATION_DAYS     -> NEAR_EXPIRATION
//   otherwise                               -> AVAILABLE
void ResourceBatchService::reevaluateBatchStatus(const Date& today) {
    if (!today.isValid()) return;

    std::vector<Batch>& batches = burger.getInventory().getBatches();
    std::vector<Batch>::iterator it;
    for (it = batches.begin(); it != batches.end(); ++it) {
        const long daysLeft = daysBetween(today, it->getExpirationDate());

        if (it->getQuantity() <= 0.0) {
            it->setStatus(BatchStatus::DEPLETED);
        } else if (daysLeft < 0) {
            it->setStatus(BatchStatus::EXPIRED);
        } else if (daysLeft <= NEAR_EXPIRATION_DAYS) {
            it->setStatus(BatchStatus::NEAR_EXPIRATION);
        } else {
            it->setStatus(BatchStatus::AVAILABLE);
        }
    }
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