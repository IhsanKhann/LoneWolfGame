// ActionQueue.cpp - Implementation samples

#include "ActionQueue.h"

// StatChangeCommand Implementation
StatChangeCommand::StatChangeCommand(Stats* stats, const StatEffect& effect, const std::string& desc)
    : targetStats(stats), appliedEffect(effect), reverseEffect(effect.reverse()), description(desc) {}

void StatChangeCommand::execute() {
    if (targetStats) {
        targetStats->applyEffect(appliedEffect);
    }
}

void StatChangeCommand::undo() {
    if (targetStats) {
        targetStats->applyEffect(reverseEffect);
    }
}

std::string StatChangeCommand::getDescription() const {
    return description;
}

// ActionNode Implementation
ActionNode::ActionNode(Command* cmd, const std::string& name, const std::string& desc)
    : command(cmd), actionName(name), description(desc), next(nullptr) {}

ActionNode::~ActionNode() {
    delete command;
}

// ActionQueue Implementation
ActionQueue::ActionQueue() : head(nullptr), tail(nullptr), size(0) {}

ActionQueue::~ActionQueue() {
    clear();
}

void ActionQueue::enqueue(Command* command, const std::string& actionName, const std::string& description) {
    if (size >= MAX_BUFFER_SIZE) {
        removeOldest();
    }
    
    ActionNode* newNode = new ActionNode(command, actionName, description);
    
    if (isEmpty()) {
        head = tail = newNode;
        newNode->next = newNode; // Circular
    } else {
        newNode->next = head;
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

bool ActionQueue::executeAndTrack(Command* command, const std::string& actionName, const std::string& description) {
    if (!command) return false;
    
    command->execute();
    enqueue(command, actionName, description);
    return true;
}

bool ActionQueue::undoLast(std::string& outName, std::string& outDescription) {
    if (isEmpty()) return false;
    
    outName = tail->actionName;
    outDescription = tail->description;
    
    tail->command->undo();
    
    if (size == 1) {
        delete head;
        head = tail = nullptr;
        size = 0;
    } else {
        ActionNode* current = head;
        while (current->next != tail) {
            current = current->next;
        }
        delete tail;
        tail = current;
        tail->next = head;
        size--;
    }
    
    return true;
}

bool ActionQueue::dequeue(std::string& outName, std::string& outDescription) {
    return undoLast(outName, outDescription);
}

bool ActionQueue::isEmpty() const {
    return size == 0;
}

int ActionQueue::getSize() const {
    return size;
}

void ActionQueue::clear() {
    while (!isEmpty()) {
        std::string dummy1, dummy2;
        undoLast(dummy1, dummy2);
    }
}

std::string ActionQueue::peek() const {
    if (isEmpty()) return "";
    return tail->actionName;
}

void ActionQueue::removeOldest() {
    if (isEmpty()) return;
    
    if (size == 1) {
        delete head;
        head = tail = nullptr;
        size = 0;
    } else {
        ActionNode* oldHead = head;
        head = head->next;
        tail->next = head;
        delete oldHead;
        size--;
    }
}