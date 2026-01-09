#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "Node.h"
#include "Event.h"
#include <unordered_map>
#include <string>
#include <vector>

// NOTE: Choice struct is defined in Node.h (included above)
// Do NOT redefine it here!

class DecisionTree {
private:
    std::unordered_map<int, Node*> nodes;
    int currentNodeId;

public:
    DecisionTree();
    ~DecisionTree();

    void loadNodes();
    const Node* getCurrentNode() const;
    
    // NavigateToNode as described in Algorithm 1
    bool navigateToNode(int targetID);
    
    bool makeChoice(int choiceIndex);
    void reset();
    void setCurrentNode(int nodeId);

    void createNode(int id, const std::string& text, bool isEnding = false);
    
    // Updated to support Choice struct with effects
    void addChoice(int nodeId, const std::string& text, int nextNodeId, const std::vector<StatEffect>& effects = {});
    void addTrigger(int nodeId, int eventId);
    void setNodeEndingType(int nodeId, const std::string& type);

    void generateDotFile(const std::string& filename) const;
};

#endif