#ifndef BATCH_H
#define BATCH_H

#include "Date.h"
#include "BatchStatus.h"

class Batch {
private:
    int id;
    int resourceId;
    int shipmentId;
    double quantity;
    double purchasePrice;
    Date expirationDate;
    BatchStatus status;

public:
    Batch();
    Batch(const Batch& other);
    Batch& operator=(const Batch& other);
    virtual ~Batch();

    int getId() const;
    int getResourceId() const;
    int getShipmentId() const;
    double getQuantity() const;
    double getPurchasePrice() const;
    const Date& getExpirationDate() const;
    BatchStatus getStatus() const;

    void setId(int id);
    void setResourceId(int resourceId);
    void setShipmentId(int shipmentId);
    void setQuantity(double quantity);
    void setPurchasePrice(double price);
    void setExpirationDate(const Date& date);
    void setStatus(BatchStatus status);
};

#endif