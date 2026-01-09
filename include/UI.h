#pragma once
#include "Stats.h"
#include "Inventory.h"
#include "DecisionTree.h"
#include "Event.h"
#include "GameState.h"
#include "ActionQueue.h"
#include <string>
#include <vector>

// UIManager class as described in Ch 7.4 (implemented as namespace for simplicity)
namespace UIManager {
    // Core rendering methods
    void render(GameState& state, DecisionTree& story, std::vector<Event>& eventLog, 
                int& selectedChoice, GameStateStack& history, ActionQueue& actionQueue);
    
    // Individual panels (as described in Ch 7)
    void displayStatsPanel(const Stats& stats, int day, int packSize, const std::string& weather);
    void displayNodeGUI(const Node* node, int& selectedChoice);
    void showInventoryGUI(Inventory* inventory, Stats& stats);
    void displayEventGUI(const std::string& text);
    void displayEventLog(const std::vector<Event>& events);
    
    // NEW: Action controls panel for undo/redo
    void displayActionControls(GameStateStack& history, ActionQueue& actionQueue, 
                               bool& undoRequested, bool& clearHistoryRequested);
    
    bool displayWelcomeScreen(bool& startGame);
    void displayEndingGUI(const std::string& text);
}

// Backward compatibility
class UI {
public:
    static void displayStatsPanel(const Stats& stats, int day, int packSize, const std::string& weather) {
        UIManager::displayStatsPanel(stats, day, packSize, weather);
    }
    static void displayNodeGUI(const Node* node, int& selectedChoice) {
        UIManager::displayNodeGUI(node, selectedChoice);
    }
    static void showInventoryGUI(Inventory* inventory, Stats& stats) {
        UIManager::showInventoryGUI(inventory, stats);
    }
    static void displayEventGUI(const std::string& text) {
        UIManager::displayEventGUI(text);
    }
    static void displayEventLog(const std::vector<Event>& events) {
        UIManager::displayEventLog(events);
    }
    static void displayActionControls(GameStateStack& history, ActionQueue& actionQueue, 
                                     bool& undoRequested, bool& clearHistoryRequested) {
        UIManager::displayActionControls(history, actionQueue, undoRequested, clearHistoryRequested);
    }
    static bool displayWelcomeScreen(bool& startGame) {
        return UIManager::displayWelcomeScreen(startGame);
    }
    static void displayEndingGUI(const std::string& text) {
        UIManager::displayEndingGUI(text);
    }
};