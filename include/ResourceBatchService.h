#ifndef RESOURCEBATCHSERVICE_H
#define RESOURCEBATCHSERVICE_H

#include <string>
#include <vector>
#include "Date.h"
#include "Batch.h"

class Burger;

class ResourceBatchService {
private:
    Burger& burger;
    int nextBatchId;

public:
    explicit ResourceBatchService(Burger& burger): burger(burger), nextBatchId(1) {}

    ResourceBatchService(const ResourceBatchService&) = delete;
    ResourceBatchService& operator=(const ResourceBatchService&) = delete;
    ~ResourceBatchService();

    // ---- Resources ----
    int createResource(const std::string& name, const std::string& unit, double purchasePrice);

    // ---- Suppliers ----
    int createSupplier(const std::string& name, const std::string& cnpj,
        const std::string& contact, const std::string& deliveryTerms);

    // ---- Shipments and Batches ----
    int createShipment(int supplierId, const Date& receiptDate);
    std::vector<int> openBatches(int shipmentId);
    std::vector<Batch> getPriorityBatches() const;
};

#endif
