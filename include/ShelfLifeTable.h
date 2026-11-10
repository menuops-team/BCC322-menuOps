#ifndef SHELFLIFETABLE_H
#define SHELFLIFETABLE_H

#include <map>

class ShelfLifeTable {
private:
    //       <ID, LifeDays>
    std::map<int, int> shelfLifeDays;

public:
    ShelfLifeTable();
    ShelfLifeTable(const ShelfLifeTable& other);
    virtual ~ShelfLifeTable() = default;
    ShelfLifeTable& operator=(const ShelfLifeTable& other);

    void setShelfLife(int resourceId, int days);
    int getShelfLife(int resourceId) const;

    bool containsResource(int resourceId) const;
    bool removeResource(int resourceId);

    void clear();
};

#endif
