#include "Node.h"
#include <utility>

// ============================================================
// Node Implementation
// ============================================================

Node::Node(int id, const std::string& text, bool isEnding)
    : id(id), text(text), ending(isEnding), endingType("") {}

int Node::getId() const { 
    return id; 
}

std::string Node::getText() const { 
    return text; 
}

bool Node::isEndingNode() const { 
    return ending; 
}

std::string Node::getEndingType() const { 
    return endingType; 
}

void Node::addChoice(const std::string& text, int nextNodeId, const std::vector<StatEffect>& effects) {
    choicesWithEffects.push_back(Choice(text, nextNodeId, effects));
}

void Node::addTrigger(int eventId) {
    triggers.push_back(eventId);
}

void Node::setEndingType(const std::string& type) {
    endingType = type;
}

const std::vector<Choice>& Node::getChoicesWithEffects() const {
    return choicesWithEffects;
}

std::vector<std::pair<std::string, int>> Node::getChoices() const {
    std::vector<std::pair<std::string, int>> result;
    for (const auto& choice : choicesWithEffects) {
        result.push_back(std::make_pair(choice.text, choice.targetNodeId));
    }
    return result;
}

const std::vector<int>& Node::getTriggers() const {
    return triggers;
}