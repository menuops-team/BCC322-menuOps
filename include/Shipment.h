 #ifndef SHIPMENT_H
#define SHIPMENT_H

#include <vector>
#include "Date.h"

class Shipment {
private:
    int id;
    int supplierId;
    Date receiptDate;
    bool accepted;

    std::vector<int> resourceIds;
    std::vector<double> quantities;
public:
    Shipment();
    Shipment(const Shipment& other);
    Shipment(int id, int supplierId, const Date& receiptDate);
    Shipment& operator=(const Shipment& other);
    virtual ~Shipment();

    int getId() const;
    int getSupplierId() const;
    Date getReceiptDate() const;
    bool isAccepted() const;

    void setId(int id);
    void setSupplierId(int supplierId);
    void setReceiptDate(const Date& receiptDate);
    void setAccepted(bool accepted);

    void addResource(int resourceId, double quantity);

    const std::vector<int>& getResourceIds() const;
    const std::vector<double>& getQuantities() const; 
};

#endif
