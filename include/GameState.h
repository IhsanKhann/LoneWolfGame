#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Stats.h"
#include "Inventory.h"

// Centralized GameState struct as described in Ch 6.3
struct GameState {
    Stats stats;
    int currentNodeId;
    int day;
    int packSize;
    Inventory* inventory;
    
    GameState() : currentNodeId(1), day(1), packSize(1), inventory(nullptr) {}
};

// Stack node for game state history (undo functionality)
struct GameStateNode {
    int currentNodeId;
    Stats stats;
    Inventory* inventory;
    int day;
    int packSize;
    GameStateNode* next;
    
    GameStateNode(int nodeId, const Stats& s, Inventory* inv, int d, int pack);
    ~GameStateNode();
};

// Stack for undo history as described in Ch 6
class GameStateStack {
public:
    GameStateStack();
    ~GameStateStack();
    
    void push(int nodeId, const Stats& stats, Inventory* inventory, int day, int packSize);
    bool pop(int& outNodeId, Stats& outStats, Inventory*& outInventory, int& outDay, int& outPackSize);
    
    // Undo implementation (Algorithm 3)
    bool undo(GameState& outState);
    
    int getSize() const;
    bool isEmpty() const;
    void clear();

private:
    GameStateNode* top;
    int size;
    static const int MAX_SIZE = 5;
    
    Inventory* copyInventory(Inventory* original);
};

#endif