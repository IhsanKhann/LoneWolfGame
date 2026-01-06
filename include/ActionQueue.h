#ifndef ACTIONQUEUE_H
#define ACTIONQUEUE_H

#include <string>

// Queue node for action sequences
struct ActionNode {
    std::string actionName;
    std::string description;
    ActionNode* next;
    
    ActionNode(const std::string& name, const std::string& desc);
};

class ActionQueue {
public:
    ActionQueue();
    ~ActionQueue();
    
    void enqueue(const std::string& actionName, const std::string& description);
    bool dequeue(std::string& outName, std::string& outDescription);
    bool isEmpty() const;
    int getSize() const;
    void clear();
    
    std::string peek() const;

private:
    ActionNode* front;
    ActionNode* rear;
    int size;
};

#endif