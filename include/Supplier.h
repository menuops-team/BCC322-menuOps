#ifndef SUPPLIER_H
#define SUPPLIER_H

#include <string>

class Supplier {
private:
    int id;
    std::string name;
    std::string cnpj;
    std::string contact;
    std::string deliveryTerms;
    bool active;

    Supplier();
    Supplier(int id, const std::string& name,
             const std::string& cnpj,
             const std::string& contact,
             const std::string& deliveryTerms);
    Supplier(const Supplier& other);
public:
    Supplier& operator=(const Supplier& other);
    virtual ~Supplier();

    int getId() const;
    std::string getName() const;
    std::string getCnpj() const;
    std::string getContact() const;
    std::string getDeliveryTerms() const;
    bool isActive() const;

    void setId(int id);
    void setName(const std::string& name);
    void setCnpj(const std::string& cnpj);
    void setContact(const std::string& contact);
    void setDeliveryTerms(const std::string& deliveryTerms);
    void setActive(bool active);
};

#endif
