#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Stats.h"
#include "Inventory.h"

// Stack node for game state history
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

class GameStateStack {
public:
    GameStateStack();
    ~GameStateStack();
    
    void push(int nodeId, const Stats& stats, Inventory* inventory, int day, int packSize);
    bool pop(int& outNodeId, Stats& outStats, Inventory*& outInventory, int& outDay, int& outPackSize);
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