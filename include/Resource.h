 #ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

class Resource {
private:
    int id;
    std::string name;
public:
    Resource() = delete; // Prevent empty construction
    Resource(const Resource& other);
    Resource(int id, const std::string& name);
    Resource& operator=(const Resource& other);
    virtual ~Resource();

    int getId() const;
    std::string getName() const;

    void setId(int id);
    void setName(const std::string& name);
};

#endif
