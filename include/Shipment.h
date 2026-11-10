 #ifndef SHIPMENT_H
#define SHIPMENT_H

#include <vector>
#include "Date.h"
#include "Resource.h"
#include "Supplier.h"

class Shipment {
private:
    int id;
    int supplierId;
    Date receiptDate;
    bool accepted;

    std::vector<int> resourceIds;
    std::vector<double> quantities;
    
    Shipment();
    Shipment(int id, int supplierId, const Date& receiptDate);
    Shipment(const Shipment& other);
public:
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
