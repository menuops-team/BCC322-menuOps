#include "Resource.h"

#include <stdexcept>

using namespace std;

Resource::Resource(const Resource& other)
    : id(other.id), name(other.name) {}

Resource::Resource(int id, const string& name)
    : id(1), name("resource") {
    setId(id);
    setName(name);
}

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
    if (id <= 0) {
        throw invalid_argument("Resource ID must be positive");
    }
    this->id = id;
}
void Resource::setName(const string& name) {
    if (name.find_first_not_of(" \t\n\r\f\v") == string::npos) {
        throw invalid_argument("Resource name must not be empty or whitespace-only");
    }
    this->name = name;
}
