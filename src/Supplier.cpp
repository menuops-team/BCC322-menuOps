#include "../include/Supplier.h"
using namespace std;

Supplier::Supplier(const Supplier& other)
    : id(other.id), name(other.name), cnpj(other.cnpj), contact(other.contact), deliveryTerms(other.deliveryTerms), active(other.active) {}

Supplier::Supplier(int id, const string& name, const string& cnpj, const string& contact, const string& deliveryTerms) 
    : id(id), name(name), cnpj(cnpj), contact(contact), deliveryTerms(deliveryTerms), active(true) {}
 
Supplier& Supplier::operator=(const Supplier& other) {
    if (this != &other) {
        id = other.id;
        name = other.name;
        cnpj = other.cnpj;
        contact = other.contact;
        deliveryTerms = other.deliveryTerms;
        active = other.active;
    }
    return *this;
}

int Supplier::getId() const {
    return id;
}

string Supplier::getName() const {
    return name;
}

string Supplier::getCnpj() const {
    return cnpj;
}

string Supplier::getContact() const {
    return contact;
}

string Supplier::getDeliveryTerms() const {
    return deliveryTerms;
}

bool Supplier::isActive() const {
    return active;
}

void Supplier::setId(int id) {
    this->id = id;
}

void Supplier::setName(const string& name) {
    this->name = name;
}

void Supplier::setCnpj(const string& cnpj) {
    this->cnpj = cnpj;
}

void Supplier::setContact(const string& contact) {
    this->contact = contact;
}

void Supplier::setDeliveryTerms(const string& deliveryTerms) {
    this->deliveryTerms = deliveryTerms;
}

void Supplier::setActive(bool active) {
    this->active = active;
}
