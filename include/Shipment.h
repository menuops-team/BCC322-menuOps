 #ifndef SHIPMENT_H
#define SHIPMENT_H

#include <map>
#include "Date.h"

class Shipment {
private:
    int id;
    int supplierId;
    Date receiptDate;
    bool accepted;

    std::map<int, double> resources;
public:
    Shipment() = delete;
    Shipment(const Shipment& other);
    Shipment(int id, int supplierId, const Date& receiptDate);
    Shipment& operator=(const Shipment& other);
    virtual ~Shipment() = default;

    int getId() const;
    int getSupplierId() const;
    Date getReceiptDate() const;
    bool isAccepted() const;

    void setId(int id);
    void setSupplierId(int supplierId);
    void setReceiptDate(const Date& receiptDate);
    void setAccepted(bool accepted);

    void addResource(int resourceId, double quantity);
    bool removeResource(int resourceId);

    const std::map<int, double>& getResources() const;
};

#endif
