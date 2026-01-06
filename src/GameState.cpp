#include "GameState.h"

GameStateNode::GameStateNode(int nodeId, const Stats& s, Inventory* inv, int d, int pack)
    : currentNodeId(nodeId), stats(s), inventory(inv), day(d), packSize(pack), next(nullptr) {}

GameStateNode::~GameStateNode() {
    delete inventory;
}

GameStateStack::GameStateStack() : top(nullptr), size(0) {}

GameStateStack::~GameStateStack() {
    clear();
}

void GameStateStack::push(int nodeId, const Stats& stats, Inventory* inventory, int day, int packSize) {
    // If stack is full, remove oldest state (at bottom)
    if (size >= MAX_SIZE) {
        if (top == nullptr) return;
        
        if (size == 1) {
            delete top;
            top = nullptr;
            size = 0;
        } else {
            GameStateNode* current = top;
            GameStateNode* prev = nullptr;
            while (current->next != nullptr) {
                prev = current;
                current = current->next;
            }
            if (prev != nullptr) {
                prev->next = nullptr;
            }
            delete current;
            size--;
        }
    }
    
    // Create copy of inventory
    Inventory* invCopy = copyInventory(inventory);
    
    // Push new state
    GameStateNode* newNode = new GameStateNode(nodeId, stats, invCopy, day, packSize);
    newNode->next = top;
    top = newNode;
    size++;
}

bool GameStateStack::pop(int& outNodeId, Stats& outStats, Inventory*& outInventory, int& outDay, int& outPackSize) {
    if (isEmpty()) return false;
    
    GameStateNode* temp = top;
    outNodeId = temp->currentNodeId;
    outStats = temp->stats;
    outInventory = temp->inventory;
    outDay = temp->day;
    outPackSize = temp->packSize;
    
    top = top->next;
    temp->inventory = nullptr; // Prevent deletion, ownership transferred
    delete temp;
    size--;
    
    return true;
}

int GameStateStack::getSize() const {
    return size;
}

bool GameStateStack::isEmpty() const {
    return top == nullptr;
}

void GameStateStack::clear() {
    while (top != nullptr) {
        GameStateNode* temp = top;
        top = top->next;
        delete temp;
    }
    size = 0;
}

Inventory* GameStateStack::copyInventory(Inventory* original) {
    Inventory* copy = new Inventory();
    
    InventoryNode* current = original->getHead();
    while (current != nullptr) {
        copy->addItem(current->name, current->type, current->effect, current->quantity);
        current = current->next;
    }
    
    return copy;
}