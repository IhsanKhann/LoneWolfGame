#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include "Event.h"

// ============================================================
// IMPORTANT: Choice struct MUST be defined here, not forward declared!
// ============================================================

// Choice structure with stat effects (moved from DecisionTree.h)
struct Choice {
    std::string text;
    int targetNodeId;
    std::vector<StatEffect> effects;
    
    Choice(const std::string& t, int target, const std::vector<StatEffect>& eff = {})
        : text(t), targetNodeId(target), effects(eff) {}
};

// ============================================================
// Node class
// ============================================================

class Node {
public:
    Node(int id, const std::string& text, bool isEnding = false);
    
    int getId() const;
    std::string getText() const;
    bool isEndingNode() const;
    std::string getEndingType() const;
    
    // Support for Choice struct with effects
    void addChoice(const std::string& text, int nextNodeId, const std::vector<StatEffect>& effects = {});
    void addTrigger(int eventId);
    void setEndingType(const std::string& type);
    
    // Return Choice structs instead of pairs
    const std::vector<Choice>& getChoicesWithEffects() const;
    
    // Legacy support - returns pairs for backward compatibility
    std::vector<std::pair<std::string, int>> getChoices() const;
    
    const std::vector<int>& getTriggers() const;

private:
    int id;
    std::string text;
    bool ending;
    std::string endingType;
    std::vector<Choice> choicesWithEffects;  // Uses Choice struct defined above
    std::vector<int> triggers;
};

#endif