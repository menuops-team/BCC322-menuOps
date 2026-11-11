#include "Resource.h"

using namespace std;

Resource::Resource(const Resource& other)
    : id(other.id), name(other.name) {}

Resource::Resource(int id, const string& name)
    : id(id), name(name) {}

Resource& Resource::operator=(const Resource& other) {
    if (this != &other) {
        id = other.id;
        name = other.name;
    }
    return *this;
}

Resource::~Resource() = default;

int Resource::getId() const {
    return id;
}
string Resource::getName() const {
    return name;
}


void Resource::setId(int id) {
    this->id = id;
}
void Resource::setName(const string& name) {
    this->name = name;
}
