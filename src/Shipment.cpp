#include "../include/Shipment.h"
using namespace std;

Shipment::Shipment(const Shipment& other) : id(other.id), supplierId(other.supplierId), receiptDate(other.receiptDate), accepted(other.accepted), resources(other.resources) {}

Shipment::Shipment(int id, int supplierId, const Date& receiptDate) : id(id), supplierId(supplierId), receiptDate(receiptDate), accepted(false) {}

Shipment& Shipment::operator=(const Shipment& other) {
    if (this != &other) {
        id = other.id;
        supplierId = other.supplierId;
        receiptDate = other.receiptDate;
        accepted = other.accepted;
        resources = other.resources;
    }
    return *this;
}

int Shipment::getId() const {
    return id;
}

int Shipment::getSupplierId() const {
    return supplierId;
}

Date Shipment::getReceiptDate() const {
    return receiptDate;
}

bool Shipment::isAccepted() const {
    return accepted;
}

void Shipment::setId(int id) {
    this->id = id;
}

void Shipment::setSupplierId(int supplierId) {
    this->supplierId = supplierId;
}

void Shipment::setReceiptDate(const Date& receiptDate) {
    this->receiptDate = receiptDate;
}

void Shipment::setAccepted(bool accepted) {
    this->accepted = accepted;
}

void Shipment::addResource(int resourceId, double quantity) {
    resources[resourceId] += quantity;
}

bool Shipment::removeResource(int resourceId) {
    return resources.erase(resourceId) > 0;
}

const std::map<int, double>& Shipment::getResources() const {
    return resources;
}
