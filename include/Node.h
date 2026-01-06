#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

class Node {
public:
    Node(int id, const std::string& text, bool isEnding = false);
    
    int getId() const;
    std::string getText() const;
    bool isEndingNode() const;
    std::string getEndingType() const;
    
    void addChoice(const std::string& text, int nextNodeId);
    void addTrigger(int eventId);
    void setEndingType(const std::string& type);
    
    const std::vector<std::pair<std::string, int>>& getChoices() const;
    const std::vector<int>& getTriggers() const;

private:
    int id;
    std::string text;
    bool ending;
    std::string endingType;
    std::vector<std::pair<std::string, int>> choices;
    std::vector<int> triggers;
};

#endif