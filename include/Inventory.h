#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>

// Singly Linked List implementation for inventory
struct InventoryNode {
    std::string name;
    std::string type;
    int effect;
    int quantity;
    InventoryNode* next;
    
    InventoryNode(const std::string& n, const std::string& t, int e, int q = 1);
};

class Inventory {
public:
    Inventory();
    ~Inventory();
    
    bool addItem(const std::string& name, const std::string& type, int effect, int quantity = 1);
    bool useItem(const std::string& name, int& outEffect, std::string& outType);
    bool hasItem(const std::string& name) const;
    int getSize() const;
    bool isFull() const;
    void clear();
    
    InventoryNode* getHead() const;

private:
    InventoryNode* head;
    int size;
    static const int MAX_SIZE = 10;
};

#endif