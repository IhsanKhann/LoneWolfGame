#pragma once
#include "Stats.h"
#include "Inventory.h"
#include "DecisionTree.h"
#include "Event.h"
#include <string>
#include <vector>

class UI {
public:
    // Panels
    static void displayStatsPanel(const Stats& stats, int day, int packSize, const std::string& weather);
    static void displayNodeGUI(const Node* node, int& selectedChoice);
    static void showInventoryGUI(Inventory* inventory, Stats& stats);
    static void displayEventGUI(const std::string& text);
    static void displayEventLog(const std::vector<Event>& events);
    static bool displayWelcomeScreen(bool& startGame);
    static void displayEndingGUI(const std::string& text);
};
