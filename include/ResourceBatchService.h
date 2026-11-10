#ifndef RESOURCEBATCHSERVICE_H
#define RESOURCEBATCHSERVICE_H

#include <string>
#include <vector>
#include "Date.h"
#include "BatchStatus.h"
#include "Resource.h"
#include "Supplier.h"
#include "Shipment.h"
#include "Batch.h"

class ResourceBatchService {
public:
    virtual ~ResourceBatchService() {}

    // ---- Resources ----
    virtual int createResource(const std::string& name, const std::string& unit,
        double purchasePrice) = 0;
    virtual void updateResource(const Resource& resource) = 0;
    virtual void deactivateResource(int resourceId) = 0;
    virtual void updatePurchasePrice(int resourceId, double price) = 0;

    // ---- Suppliers ----
    virtual int createSupplier(const std::string& name, const std::string& cnpj,
        const std::string& contact,const std::string& deliveryTerms) = 0;
    virtual void updateSupplier(const Supplier& supplier) = 0;
    virtual void deactivateSupplier(int supplierId) = 0;

    // ---- Shipments and Batches ----
    virtual int createShipment(int supplierId,
        const Date& receiptDate) = 0;
    virtual std::vector<int> openBatches(int shipmentId) = 0;
    virtual std::vector<Batch> getBatches() const = 0;
    virtual std::vector<Batch> getBatches(BatchStatus status) const = 0;
    virtual void reevaluateBatchStatus(const Date& today) = 0;
    virtual void registerBatchConsumption(int batchId, double quantity, const Date& date) = 0;
    virtual void registerBatchLoss(int batchId, double quantity, const Date& date, const std::string& cause) = 0;
    virtual std::vector<Batch> getPriorityBatches() const = 0;

protected:
    ResourceBatchService() {}
    ResourceBatchService(const ResourceBatchService&) = delete;
    ResourceBatchService& operator=(const ResourceBatchService&) = delete;
};

#endif
