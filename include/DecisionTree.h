#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "Node.h"
#include <unordered_map>
#include <string>

class DecisionTree {
private:
    std::unordered_map<int, Node*> nodes;
    int currentNodeId;

public:
    DecisionTree();
    ~DecisionTree();

    void loadNodes();
    const Node* getCurrentNode() const;
    bool makeChoice(int choiceIndex);
    void reset();
    void setCurrentNode(int nodeId);

    void createNode(int id, const std::string& text, bool isEnding = false);
    void addChoice(int nodeId, const std::string& text, int nextNodeId);
    void addTrigger(int nodeId, int eventId);
    void setNodeEndingType(int nodeId, const std::string& type);

    void generateDotFile(const std::string& filename) const;
};

#endif
