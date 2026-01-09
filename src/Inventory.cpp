#include "Inventory.h"

// No changes needed - implementation matches report

InventoryNode::InventoryNode(const std::string& n, const std::string& t, int e, int q)
    : name(n), type(t), effect(e), quantity(q), next(nullptr) {}

Inventory::Inventory() : head(nullptr), size(0) {}

Inventory::~Inventory() {
    clear();
}

bool Inventory::addItem(const std::string& name, const std::string& type, int effect, int quantity) {
    if (isFull()) {
        return false;
    }
    
    // Check if item already exists
    InventoryNode* current = head;
    while (current != nullptr) {
        if (current->name == name) {
            current->quantity += quantity;
            return true;
        }
        current = current->next;
    }
    
    // Add new item at head
    InventoryNode* newNode = new InventoryNode(name, type, effect, quantity);
    newNode->next = head;
    head = newNode;
    size++;
    return true;
}

bool Inventory::useItem(const std::string& name, int& outEffect, std::string& outType) {
    if (head == nullptr) return false;
    
    // Special case: removing from head
    if (head->name == name) {
        outEffect = head->effect;
        outType = head->type;
        head->quantity--;
        
        if (head->quantity <= 0) {
            InventoryNode* temp = head;
            head = head->next;
            delete temp;
            size--;
        }
        return true;
    }
    
    // Search for item in rest of list
    InventoryNode* current = head;
    while (current->next != nullptr) {
        if (current->next->name == name) {
            outEffect = current->next->effect;
            outType = current->next->type;
            current->next->quantity--;
            
            if (current->next->quantity <= 0) {
                InventoryNode* temp = current->next;
                current->next = current->next->next;
                delete temp;
                size--;
            }
            return true;
        }
        current = current->next;
    }
    
    return false;
}

bool Inventory::hasItem(const std::string& name) const {
    InventoryNode* current = head;
    while (current != nullptr) {
        if (current->name == name) return true;
        current = current->next;
    }
    return false;
}

int Inventory::getSize() const {
    return size;
}

bool Inventory::isFull() const {
    return size >= MAX_SIZE;
}

void Inventory::clear() {
    while (head != nullptr) {
        InventoryNode* temp = head;
        head = head->next;
        delete temp;
    }
    size = 0;
}

InventoryNode* Inventory::getHead() const {
    return head;
}