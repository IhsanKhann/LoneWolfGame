#include "../include/UI.h"
#include <imgui.h>
#include <iostream>

// ============================================================
// UPDATED: Added complete undo/redo UI controls
// Added action history display
// Added visual feedback for all actions
// Optimized window sizes for better content visibility
// ============================================================

// UIManager implementation (Ch 7.4)
namespace UIManager {

// Main render loop (orchestrates all UI elements) - UPDATED with undo controls
void render(GameState& state, DecisionTree& story, std::vector<Event>& eventLog, 
            int& selectedChoice, GameStateStack& history, ActionQueue& actionQueue) {
    displayStatsPanel(state.stats, state.day, state.packSize, "Winter");
    displayNodeGUI(story.getCurrentNode(), selectedChoice);
    showInventoryGUI(state.inventory, state.stats);
    displayEventLog(eventLog);
    
    // NEW: Display action controls with undo/redo functionality
    bool undoRequested = false;
    bool clearHistoryRequested = false;
    displayActionControls(history, actionQueue, undoRequested, clearHistoryRequested);
}

// Stats panel with progress bars - INCREASED SIZE
void displayStatsPanel(const Stats& stats, int day, int packSize, const std::string& weather) {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(320, 420), ImGuiCond_Once);  // Increased from 280x350
    ImGui::Begin("Wolf Status", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 12));  // More spacing
    
    // Header info with larger font
    ImGui::Text("Day: %d", day);
    ImGui::Text("Pack Size: %d", packSize);
    ImGui::Text("Weather: %s", weather.c_str());
    ImGui::Separator();
    ImGui::Spacing();
    
    // Progress bars with better sizing and labels
    ImGui::Text("Health: %d / 100", stats.getHealth());
    float healthPercent = stats.getHealth() / 100.0f;
    ImVec4 healthColor = healthPercent > 0.5f ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : 
                         (healthPercent > 0.2f ? ImVec4(1.0f, 1.0f, 0.0f, 1.0f) : 
                          ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, healthColor);
    ImGui::ProgressBar(healthPercent, ImVec2(-1, 25));  // Taller bars
    ImGui::PopStyleColor();
    ImGui::Spacing();
    
    ImGui::Text("Hunger: %d / 100", stats.getHunger());
    float hungerPercent = (100 - stats.getHunger()) / 100.0f;
    ImVec4 hungerColor = hungerPercent > 0.5f ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : 
                         (hungerPercent > 0.2f ? ImVec4(1.0f, 1.0f, 0.0f, 1.0f) : 
                          ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, hungerColor);
    ImGui::ProgressBar(hungerPercent, ImVec2(-1, 25));
    ImGui::PopStyleColor();
    ImGui::Spacing();
    
    ImGui::Text("Stamina: %d / 100", stats.getStamina());
    float staminaPercent = stats.getStamina() / 100.0f;
    ImVec4 staminaColor = staminaPercent > 0.5f ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : 
                          (staminaPercent > 0.2f ? ImVec4(1.0f, 1.0f, 0.0f, 1.0f) : 
                           ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, staminaColor);
    ImGui::ProgressBar(staminaPercent, ImVec2(-1, 25));
    ImGui::PopStyleColor();
    ImGui::Spacing();
    
    ImGui::Text("Pack Status: %d / 100", stats.getPackStatus());
    ImGui::ProgressBar(stats.getPackStatus() / 100.0f, ImVec2(-1, 25));
    ImGui::Spacing();
    
    ImGui::Text("Morale: %d / 100", stats.getMorale());
    ImGui::ProgressBar(stats.getMorale() / 100.0f, ImVec2(-1, 25));
    ImGui::Spacing();
    
    ImGui::Text("Strength: %d / 100", stats.getStrength());
    ImGui::ProgressBar(stats.getStrength() / 100.0f, ImVec2(-1, 25));
    ImGui::Spacing();
    
    ImGui::Separator();
    ImGui::Text("Experience Points: %d", stats.getXP());
    ImGui::Spacing();
    
    // Warning messages for critical stats - more visible
    ImGui::Separator();
    if (stats.getHealth() < 20) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "⚠ CRITICAL HEALTH!");
    }
    if (stats.getHunger() > 80) {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "⚠ STARVING!");
    }
    if (stats.getStamina() < 20) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "⚠ EXHAUSTED!");
    }

    ImGui::PopStyleVar();
    ImGui::End();
}

// Story node panel - INCREASED SIZE for better text wrapping
void displayNodeGUI(const Node* node, int& selectedChoice) {
    if (!node) return;

    ImGui::SetNextWindowPos(ImVec2(340, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(750, 500), ImGuiCond_Once);  // Increased from 700x400
    ImGui::Begin("Wilderness Log", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 10));
    
    // Story text with better wrapping area
    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 730);
    ImGui::TextWrapped("%s", node->getText().c_str());
    ImGui::PopTextWrapPos();
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Choice buttons with better sizing
    const auto& choices = node->getChoices();
    for (size_t i = 0; i < choices.size(); ++i) {
        std::string buttonLabel = std::string(1, 'A' + i) + ". " + choices[i].first;
        if (ImGui::Button(buttonLabel.c_str(), ImVec2(730, 50))) {  // Taller buttons
            selectedChoice = static_cast<int>(i);
        }
        ImGui::Spacing();
    }
    
    if (node->isEndingNode()) {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "★ ENDING REACHED ★");
        ImGui::TextWrapped("%s", node->getEndingType().c_str());
        ImGui::PopStyleVar();
    }

    ImGui::PopStyleVar();
    ImGui::End();
}

// Inventory panel - INCREASED SIZE
void showInventoryGUI(Inventory* inventory, Stats& stats) {
    ImGui::SetNextWindowPos(ImVec2(1100, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, 420), ImGuiCond_Once);  // Increased from 260x350
    ImGui::Begin("Inventory", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
    
    ImGui::Text("Capacity: %d / 10 items", inventory->getSize());
    ImGui::ProgressBar(inventory->getSize() / 10.0f, ImVec2(-1, 20));
    ImGui::Separator();
    ImGui::Spacing();

    if (inventory->getSize() == 0) {
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "   [Empty Inventory]");
        ImGui::Spacing();
        ImGui::TextWrapped("Collect items during your journey to survive.");
    } else {
        ImGui::BeginChild("ItemList", ImVec2(0, 300), true);
        
        InventoryNode* current = inventory->getHead();
        int itemIndex = 1;
        while (current != nullptr) {
            ImGui::PushID(itemIndex);
            
            // Item display with icon
            std::string itemIcon = (current->type == "FOOD") ? "🍖" : "🌿";
            ImGui::Text("%s %s", itemIcon.c_str(), current->name.c_str());
            
            // Details on same line
            ImGui::SameLine(200);
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "x%d", current->quantity);
            
            // Effect info
            ImGui::Text("   Effect: +%d %s", current->effect, current->type.c_str());
            
            // Use button
            std::string buttonLabel = "Use Item##" + std::to_string(itemIndex);
            if (ImGui::Button(buttonLabel.c_str(), ImVec2(-1, 30))) {
                int effect;
                std::string type;
                if (current->type == "FOOD") {
                    stats.setHunger(stats.getHunger() - current->effect);
                    ImGui::OpenPopup("ItemUsed");
                } else if (current->type == "HERB") {
                    stats.setHealth(stats.getHealth() + current->effect);
                    ImGui::OpenPopup("ItemUsed");
                }
                inventory->useItem(current->name, effect, type);
                ImGui::PopID();
                break; // Refresh needed after use
            }
            
            ImGui::Separator();
            ImGui::Spacing();
            
            current = current->next;
            itemIndex++;
            ImGui::PopID();
        }
        
        ImGui::EndChild();
        
        // Feedback popup
        if (ImGui::BeginPopup("ItemUsed")) {
            ImGui::Text("✓ Item used successfully!");
            ImGui::EndPopup();
        }
    }

    ImGui::PopStyleVar();
    ImGui::End();
}

// Single event window (floating)
void displayEventGUI(const std::string& text) {
    ImGui::SetNextWindowSize(ImVec2(600, 80), ImGuiCond_Once);
    ImGui::Begin("Event", nullptr, ImGuiWindowFlags_NoCollapse);
    ImGui::TextWrapped("%s", text.c_str());
    ImGui::End();
}

// Event log - INCREASED SIZE for more events
void displayEventLog(const std::vector<Event>& events) {
    ImGui::SetNextWindowPos(ImVec2(340, 520), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(750, 250), ImGuiCond_Once);  // Increased from 700x220
    ImGui::Begin("Event Log", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 6));
    
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Recent Events:");
    ImGui::Separator();
    
    ImGui::BeginChild("EventScrollRegion", ImVec2(0, 190), true);

    // Show most recent events (last 20 for larger window)
    int startIdx = events.size() > 20 ? events.size() - 20 : 0;
    for (int i = startIdx; i < static_cast<int>(events.size()); ++i) {
        std::string eventText = "• " + events[i].getDescription();
        ImGui::TextWrapped("%s", eventText.c_str());
        ImGui::Spacing();
    }

    // Auto-scroll to bottom
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    
    ImGui::PopStyleVar();
    ImGui::End();
}

// NEW: Action controls panel - INCREASED SIZE for better readability
void displayActionControls(GameStateStack& history, ActionQueue& actionQueue, 
                           bool& undoRequested, bool& clearHistoryRequested) {
    ImGui::SetNextWindowPos(ImVec2(10, 440), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(320, 330), ImGuiCond_Once);  // Increased from 280x340
    ImGui::Begin("Action Controls", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 10));
    
    // Title and description
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "⏰ Time Control System");
    ImGui::Separator();
    ImGui::Spacing();
    
    // State history information
    int historySize = history.getSize();
    ImGui::Text("Game State History:");
    ImGui::Text("  %d / 5 saves available", historySize);
    ImGui::ProgressBar(historySize / 5.0f, ImVec2(-1, 22));
    ImGui::Spacing();
    
    // Action queue information
    int actionQueueSize = actionQueue.getSize();
    ImGui::Text("Action Queue:");
    ImGui::Text("  %d / 10 actions tracked", actionQueueSize);
    ImGui::ProgressBar(actionQueueSize / 10.0f, ImVec2(-1, 22));
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    // Undo button with keyboard shortcut indicator - LARGER
    bool canUndo = !history.isEmpty();
    if (!canUndo) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
    }
    
    if (ImGui::Button("⟲ UNDO LAST CHOICE\n(Press U Key)", ImVec2(-1, 55))) {
        if (canUndo) {
            undoRequested = true;
        }
    }
    
    if (!canUndo) {
        ImGui::PopStyleVar();
    }
    
    if (ImGui::IsItemHovered() && !canUndo) {
        ImGui::SetTooltip("No previous states available to restore");
    } else if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Restore previous game state\nKeyboard Shortcut: U");
    }
    
    ImGui::Spacing();
    
    // Last action preview with wrapping
    ImGui::Separator();
    if (!actionQueue.isEmpty()) {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Last Action Taken:");
        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 300);
        ImGui::TextWrapped("\"%s\"", actionQueue.peek().c_str());
        ImGui::PopTextWrapPos();
    } else {
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No recent actions recorded");
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    // Clear history button
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "⚠ Advanced Options:");
    if (ImGui::Button("Clear All History", ImVec2(-1, 35))) {
        clearHistoryRequested = true;
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Remove all saved states\nWARNING: This cannot be undone!");
    }
    
    ImGui::PopStyleVar();
    ImGui::End();
}

// Welcome screen - LARGER for better presentation
bool displayWelcomeScreen(bool& startGame) {
    ImGui::SetNextWindowPos(ImVec2(340, 200), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(600, 350), ImGuiCond_Once);  // Increased from 480x260
    ImGui::Begin("Welcome to Wolf Pack Survival", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 10));
    
    ImGui::Spacing();
    ImGui::TextWrapped(
        "You are a lone wolf separated from your pack in the harsh Alaskan wilderness. "
        "Your survival depends on the choices you make. Hunt for food, navigate dangerous terrain, "
        "and interact with other wolves as you fight to stay alive through the brutal winter."
    );
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 1.0f, 1.0f), "★ Game Features:");
    ImGui::Spacing();
    ImGui::BulletText("25+ story nodes with multiple branching paths");
    ImGui::BulletText("Dynamic survival mechanics (health, hunger, stamina)");
    ImGui::BulletText("Priority-based event system for immersive gameplay");
    ImGui::BulletText("Complete undo system - press U anytime to go back");
    ImGui::BulletText("Multiple unique story endings based on your choices");
    ImGui::BulletText("Inventory management and resource gathering");
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Keyboard Controls:");
    ImGui::BulletText("U - Undo your last choice");
    ImGui::BulletText("ESC - Quit the game");
    
    ImGui::Spacing();
    ImGui::Spacing();
    
    if (ImGui::Button("🐺  START YOUR JOURNEY  🐺", ImVec2(-1, 60))) {
        startGame = true;
    }

    ImGui::PopStyleVar();
    ImGui::End();
    return startGame;
}

// Ending screen - LARGER for full ending text
void displayEndingGUI(const std::string& text) {
    ImGui::SetNextWindowPos(ImVec2(340, 200), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_Once);  // Increased from 480x220
    ImGui::Begin("Game Over", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 12));
    
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "★ Your Story Has Ended ★");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 570);
    ImGui::TextWrapped("%s", text.c_str());
    ImGui::PopTextWrapPos();
    
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Thank you for playing Wolf Pack Survival!");
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Press ESC to close the game.");

    ImGui::PopStyleVar();
    ImGui::End();
}

}