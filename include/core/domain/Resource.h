 #ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

class Resource {
private:
    int id;
    std::string name;
    std::string unit;
    double purchasePrice;
    bool active;

public:
    Resource();
    Resource(int id, const std::string& name, const std::string& unit, double purchasePrice);
    Resource(const Resource& other);
    Resource& operator=(const Resource& other);
    ~Resource();

    int getId() const;
    std::string getName() const;
    std::string getUnit() const;
    double getPurchasePrice() const;
    bool isActive() const;

    void setId(int id);
    void setName(const std::string& name);
    void setUnit(const std::string& unit);
    void setPurchasePrice(double price);
    void setActive(bool active);
};

#endif
