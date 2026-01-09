#ifndef ACTIONQUEUE_H
#define ACTIONQUEUE_H

#include <string>
#include "Stats.h"
#include "Inventory.h"

// Forward declaration
class GameStateSnapshot;

// Command pattern base class for undo/redo
class Command {
public:
    virtual ~Command() {}
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string getDescription() const = 0;
};

// Concrete command for stat changes
class StatChangeCommand : public Command {
public:
    StatChangeCommand(Stats* stats, const StatEffect& effect, const std::string& desc);
    void execute() override;
    void undo() override;
    std::string getDescription() const override;

private:
    Stats* targetStats;
    StatEffect appliedEffect;
    StatEffect reverseEffect;
    std::string description;
};

// Circular buffer node for action sequences
struct ActionNode {
    Command* command;
    std::string actionName;
    std::string description;
    ActionNode* next;
    
    ActionNode(Command* cmd, const std::string& name, const std::string& desc);
    ~ActionNode();
};

// Circular buffer implementation for undo history
class ActionQueue {
public:
    ActionQueue();
    ~ActionQueue();
    
    // Add command to circular buffer
    void enqueue(Command* command, const std::string& actionName, const std::string& description);
    
    // Execute and track action
    bool executeAndTrack(Command* command, const std::string& actionName, const std::string& description);
    
    // Undo last action (pops from history)
    bool undoLast(std::string& outName, std::string& outDescription);
    
    // Legacy support
    bool dequeue(std::string& outName, std::string& outDescription);
    bool isEmpty() const;
    int getSize() const;
    void clear();
    
    std::string peek() const;

private:
    ActionNode* head;      // Start of circular buffer
    ActionNode* tail;      // Current position
    int size;
    static const int MAX_BUFFER_SIZE = 10;
    
    void removeOldest();
};

#endif