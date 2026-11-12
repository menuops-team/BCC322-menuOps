#include "Batch.h"

#include <cmath>
#include <stdexcept>

using namespace std;

Batch::Batch(int id, int resourceId, int shipmentId, double quantity, double purchasePrice, 
    const Date& expirationDate, BatchStatus status)
    : expirationDate(expirationDate) {
    setId(id);
    setResourceId(resourceId);
    setShipmentId(shipmentId);
    setQuantity(quantity);
    setPurchasePrice(purchasePrice);
    setExpirationDate(expirationDate);
    setStatus(status);
}

Batch::Batch(const Batch& other)
    : id(other.id),
      resourceId(other.resourceId),
      shipmentId(other.shipmentId),
      quantity(other.quantity),
      purchasePrice(other.purchasePrice),
      expirationDate(other.expirationDate),
      status(other.status) {}


Batch& Batch::operator=(const Batch& other) {
    if (this != &other) {
        id = other.id;
        resourceId = other.resourceId;
        shipmentId = other.shipmentId;
        quantity = other.quantity;
        purchasePrice = other.purchasePrice;
        expirationDate = other.expirationDate;
        status = other.status;
    }
    return *this;
}

Batch::~Batch() = default;


int Batch::getId() const {
    return id;
}

int Batch::getResourceId() const {
    return resourceId;
}

int Batch::getShipmentId() const {
    return shipmentId;
}

double Batch::getQuantity() const {
    return quantity;
}

double Batch::getPurchasePrice() const {
    return purchasePrice;
}

const Date& Batch::getExpirationDate() const {
    return expirationDate;
}

BatchStatus Batch::getStatus() const {
    return status;
}


void Batch::setId(int id) {
    if (id <= 0) {
        throw invalid_argument("Batch ID must be positive");
    }
    this->id = id;
}

void Batch::setResourceId(int resourceId) {
    if (resourceId <= 0) {
        throw invalid_argument("Resource ID must be positive");
    }
    this->resourceId = resourceId;
}

void Batch::setShipmentId(int shipmentId) {
    if (shipmentId <= 0) {
        throw invalid_argument("Shipment ID must be positive");
    }
    this->shipmentId = shipmentId;
}

void Batch::setQuantity(double quantity) {
    if (!isfinite(quantity) || quantity <= 0.0) {
        throw invalid_argument("Batch quantity must be positive and finite");
    }
    this->quantity = quantity;
}

void Batch::setPurchasePrice(double price) {
    if (!isfinite(price) || price < 0.0) {
        throw invalid_argument("Batch purchase price must be non-negative and finite");
    }
    this->purchasePrice = price;
}

void Batch::setExpirationDate(const Date& date) {
    if (!date.isValid()) {
        throw invalid_argument("Batch expiration date must be valid");
    }
    this->expirationDate = date;
}

void Batch::setStatus(BatchStatus status) {
    switch (status) {
        case BatchStatus::AVAILABLE:
        case BatchStatus::NEAR_EXPIRATION:
        case BatchStatus::EXPIRED:
        case BatchStatus::DEPLETED:
            break;
        default:
            throw invalid_argument("Invalid batch status");
    }
    this->status = status;
}
