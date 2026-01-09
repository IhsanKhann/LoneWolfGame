#include "../include/UI.h"
#include <imgui.h>
#include <iostream>

// ============================================================
// CHANGES: Implemented progress bars as shown in Listing 7.1
// Added UIManager namespace as described in Ch 7.4
// Implemented render() method for orchestration
// ============================================================

// UIManager implementation (Ch 7.4)
namespace UIManager {

// Main render loop (orchestrates all UI elements)
void render(GameState& state, DecisionTree& story, std::vector<Event>& eventLog, int& selectedChoice) {
    displayStatsPanel(state.stats, state.day, state.packSize, "Winter");
    displayNodeGUI(story.getCurrentNode(), selectedChoice);
    showInventoryGUI(state.inventory, state.stats);
    displayEventLog(eventLog);
}

// Stats panel with progress bars (as shown in Listing 7.1)
void displayStatsPanel(const Stats& stats, int day, int packSize, const std::string& weather) {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(280, 350), ImGuiCond_Once);
    ImGui::Begin("Wolf Status", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Day: %d", day);
    ImGui::Text("Pack Size: %d", packSize);
    ImGui::Text("Weather: %s", weather.c_str());
    ImGui::Separator();
    
    // Progress bars as described in Listing 7.1
    ImGui::Text("Health:");
    ImGui::ProgressBar(stats.getHealth() / 100.0f, ImVec2(-1, 0));
    
    ImGui::Text("Hunger:");
    ImGui::ProgressBar((100 - stats.getHunger()) / 100.0f, ImVec2(-1, 0));
    
    ImGui::Text("Stamina:");
    ImGui::ProgressBar(stats.getStamina() / 100.0f, ImVec2(-1, 0));
    
    ImGui::Text("Pack Status:");
    ImGui::ProgressBar(stats.getPackStatus() / 100.0f, ImVec2(-1, 0));
    
    ImGui::Text("Morale:");
    ImGui::ProgressBar(stats.getMorale() / 100.0f, ImVec2(-1, 0));
    
    ImGui::Text("Strength:");
    ImGui::ProgressBar(stats.getStrength() / 100.0f, ImVec2(-1, 0));
    
    ImGui::Separator();
    ImGui::Text("XP: %d", stats.getXP());

    ImGui::End();
}

// Story node panel (center) - matches Listing 7.1 structure
void displayNodeGUI(const Node* node, int& selectedChoice) {
    if (!node) return;

    ImGui::SetNextWindowPos(ImVec2(300, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(700, 400), ImGuiCond_Once);
    ImGui::Begin("Wilderness Log", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::TextWrapped("%s", node->getText().c_str());
    ImGui::Separator();

    const auto& choices = node->getChoices();
    for (size_t i = 0; i < choices.size(); ++i) {
        if (ImGui::Button(choices[i].first.c_str(), ImVec2(680, 40))) {
            selectedChoice = static_cast<int>(i);
        }
    }
    
    if (node->isEndingNode()) {
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "ENDING: %s", node->getEndingType().c_str());
    }

    ImGui::End();
}

// Inventory panel (top-right)
void showInventoryGUI(Inventory* inventory, Stats& stats) {
    ImGui::SetNextWindowPos(ImVec2(1010, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(260, 350), ImGuiCond_Once);
    ImGui::Begin("Inventory", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (inventory->getSize() == 0) {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Empty");
    } else {
        InventoryNode* current = inventory->getHead();
        while (current != nullptr) {
            std::string label = current->name + " (" + current->type + ") x" + std::to_string(current->quantity);
            if (ImGui::Button(label.c_str(), ImVec2(240, 30))) {
                int effect;
                std::string type;
                if (current->type == "FOOD") {
                    stats.setHunger(stats.getHunger() - current->effect);
                } else if (current->type == "HERB") {
                    stats.setHealth(stats.getHealth() + current->effect);
                }
                inventory->useItem(current->name, effect, type);
                break; // Refresh needed after use
            }
            current = current->next;
        }
    }

    ImGui::End();
}

// Single event window (floating)
void displayEventGUI(const std::string& text) {
    ImGui::SetNextWindowSize(ImVec2(600, 80), ImGuiCond_Once);
    ImGui::Begin("Event", nullptr, ImGuiWindowFlags_NoCollapse);
    ImGui::TextWrapped("%s", text.c_str());
    ImGui::End();
}

// Event log (bottom-center)
void displayEventLog(const std::vector<Event>& events) {
    ImGui::SetNextWindowPos(ImVec2(300, 420), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(700, 290), ImGuiCond_Once);
    ImGui::Begin("Event Log", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // Show most recent events (last 10)
    int startIdx = events.size() > 10 ? events.size() - 10 : 0;
    for (int i = startIdx; i < static_cast<int>(events.size()); ++i) {
        ImGui::BulletText("%s", events[i].getDescription().c_str());
    }

    // Auto-scroll to bottom
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::End();
}

// Welcome screen (centered)
bool displayWelcomeScreen(bool& startGame) {
    ImGui::SetNextWindowPos(ImVec2(400, 250), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(480, 220), ImGuiCond_Once);
    ImGui::Begin("Welcome to Wolf Pack Survival", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::TextWrapped(
        "You are a lone wolf in the Alaskan wilderness. "
        "Survive the harsh winter, hunt for food, and make strategic decisions. "
        "Your choices will determine your fate."
    );
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    if (ImGui::Button("Start Game", ImVec2(150, 50))) {
        startGame = true;
    }

    ImGui::End();
    return startGame;
}

// Ending screen (centered)
void displayEndingGUI(const std::string& text) {
    ImGui::SetNextWindowPos(ImVec2(400, 250), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(480, 220), ImGuiCond_Once);
    ImGui::Begin("Game Over", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::TextWrapped("%s", text.c_str());
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Press ESC to close the game.");

    ImGui::End();
}

}