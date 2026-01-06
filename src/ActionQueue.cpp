#include "ActionQueue.h"

ActionNode::ActionNode(const std::string& name, const std::string& desc)
    : actionName(name), description(desc), next(nullptr) {}

ActionQueue::ActionQueue() : front(nullptr), rear(nullptr), size(0) {}

ActionQueue::~ActionQueue() {
    clear();
}

void ActionQueue::enqueue(const std::string& actionName, const std::string& description) {
    ActionNode* newNode = new ActionNode(actionName, description);
    
    if (isEmpty()) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    size++;
}

bool ActionQueue::dequeue(std::string& outName, std::string& outDescription) {
    if (isEmpty()) return false;
    
    ActionNode* temp = front;
    outName = temp->actionName;
    outDescription = temp->description;
    
    front = front->next;
    if (front == nullptr) {
        rear = nullptr;
    }
    
    delete temp;
    size--;
    return true;
}

bool ActionQueue::isEmpty() const {
    return front == nullptr;
}

int ActionQueue::getSize() const {
    return size;
}

void ActionQueue::clear() {
    while (!isEmpty()) {
        std::string dummy1, dummy2;
        dequeue(dummy1, dummy2);
    }
}

std::string ActionQueue::peek() const {
    if (isEmpty()) return "";
    return front->actionName;
}