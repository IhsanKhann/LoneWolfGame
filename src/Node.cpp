#include "Node.h"

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

void Node::addChoice(const std::string& text, int nextNodeId) {
    choices.push_back({text, nextNodeId});
}

void Node::addTrigger(int eventId) {
    triggers.push_back(eventId);
}

void Node::setEndingType(const std::string& type) {
    endingType = type;
}

const std::vector<std::pair<std::string, int>>& Node::getChoices() const {
    return choices;
}

const std::vector<int>& Node::getTriggers() const {
    return triggers;
}