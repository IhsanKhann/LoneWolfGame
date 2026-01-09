#include "GameState.h"

GameStateNode::GameStateNode(int nodeId, const Stats& s, Inventory* inv, int d, int pack)
    : currentNodeId(nodeId), stats(s), inventory(nullptr), day(d), packSize(pack), next(nullptr) {
    if (inv) {
        inventory = new Inventory(*inv);
    }
}

GameStateNode::~GameStateNode() {
    delete inventory;
}

GameStateStack::GameStateStack() : top(nullptr), size(0) {}

GameStateStack::~GameStateStack() {
    clear();
}

void GameStateStack::push(int nodeId, const Stats& stats, Inventory* inventory, int day, int packSize) {
    if (size >= MAX_SIZE) {
        // Remove oldest (bottom of stack)
        if (top) {
            GameStateNode* current = top;
            GameStateNode* prev = nullptr;
            while (current->next) {
                prev = current;
                current = current->next;
            }
            if (prev) {
                prev->next = nullptr;
                delete current;
                size--;
            }
        }
    }
    
    GameStateNode* newNode = new GameStateNode(nodeId, stats, inventory, day, packSize);
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
    temp->inventory = nullptr; // Transfer ownership
    delete temp;
    size--;
    
    return true;
}

bool GameStateStack::undo(GameState& outState) {
    int nodeId, day, packSize;
    Stats stats;
    Inventory* inv;
    
    if (!pop(nodeId, stats, inv, day, packSize)) {
        return false;
    }
    
    outState.currentNodeId = nodeId;
    outState.stats = stats;
    outState.day = day;
    outState.packSize = packSize;
    
    delete outState.inventory;
    outState.inventory = inv;
    
    return true;
}

int GameStateStack::getSize() const {
    return size;
}

bool GameStateStack::isEmpty() const {
    return top == nullptr;
}

void GameStateStack::clear() {
    while (!isEmpty()) {
        int dummy1, dummy2, dummy3;
        Stats dummyStats;
        Inventory* dummyInv;
        pop(dummy1, dummyStats, dummyInv, dummy2, dummy3);
        delete dummyInv;
    }
}

Inventory* GameStateStack::copyInventory(Inventory* original) {
    if (!original) return nullptr;
    Inventory* copy = new Inventory();
    // Deep copy implementation would go here
    return copy;
}