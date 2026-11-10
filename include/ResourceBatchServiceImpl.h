#ifndef RESOURCEBATCHSERVICEIMPL_H
#define RESOURCEBATCHSERVICEIMPL_H

#include "ResourceBatchService.h"
#include "Burger.h"

class ResourceBatchServiceImpl : public ResourceBatchService {
private:
    Burger& burger;
    int nextBatchId;

public:
    ResourceBatchServiceImpl(const ResourceBatchServiceImpl&) = delete;
    ResourceBatchServiceImpl& operator=(const ResourceBatchServiceImpl&) = delete;
    virtual ~ResourceBatchServiceImpl();


    int create(const Resource& resource) override;
    int create(const Supplier& supplier) override;
    int create(const Shipment& shipment) override;

    void update(const Resource& resource) override;
    void update(const Supplier& supplier) override;

    void deactivateResource(int resourceId) override;
    void deactivateSupplier(int supplierId) override;

    void updatePurchasePrice(int resourceId, double price) override;

    
    std::vector<int> openBatches(int shipmentId) override;

    std::vector<Batch> getBatches() const override;
    std::vector<Batch> getBatches(BatchStatus status) const override;

    void reevaluateBatchStatus(const Date& today) override;

    void registerBatchConsumption(
        int batchId,
        double quantity,
        const Date& date
    ) override;

    void registerBatchLoss(
        int batchId,
        double quantity,
        const Date& date,
        const std::string& cause
    ) override;

    std::vector<Batch> getPriorityBatches() const override;
};

#endif