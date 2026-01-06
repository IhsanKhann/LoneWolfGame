#pragma once
#include "Stats.h"
#include "Inventory.h"
#include "DecisionTree.h"
#include <string>

class UI {
public:
    // GUI functions
    static void displayStatsGUI(const Stats& stats);
    static void displayEventGUI(const std::string& text);
    static void displayNodeGUI(const Node* node, int& selectedChoice);
    static void showInventoryGUI(Inventory* inventory, Stats& stats);
    static void displayEndingGUI(const std::string& text);
};
