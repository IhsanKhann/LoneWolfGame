#include "../include/UI.h"
#include <imgui.h>
#include <iostream>

// Stats panel (top-left)
void UI::displayStatsPanel(const Stats& stats, int day, int packSize, const std::string& weather) {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(220, 150), ImGuiCond_Once);
    ImGui::Begin("Wolf Status", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Day: %d", day);
    ImGui::Text("Health: %d", stats.getHealth());
    ImGui::Text("Hunger: %d", stats.getHunger());
    ImGui::Text("Energy: %d", stats.getStamina());
    ImGui::Text("Pack Size: %d", packSize);
    ImGui::Text("Weather: %s", weather.c_str());

    ImGui::End();
}

// Story node panel (center)
void UI::displayNodeGUI(const Node* node, int& selectedChoice) {
    if (!node) return;

    ImGui::SetNextWindowPos(ImVec2(240, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Once);
    ImGui::Begin("Story", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::TextWrapped("%s", node->getText().c_str());
    ImGui::Separator();

    const auto& choices = node->getChoices();
    for (size_t i = 0; i < choices.size(); ++i) {
        if (ImGui::Button(choices[i].first.c_str())) {
            selectedChoice = static_cast<int>(i);
        }
    }

    ImGui::End();
}

// Inventory panel (top-right)
void UI::showInventoryGUI(Inventory* inventory, Stats& stats) {
    ImGui::SetNextWindowPos(ImVec2(1050, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(220, 300), ImGuiCond_Once);
    ImGui::Begin("Inventory", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (inventory->getSize() == 0) {
        ImGui::Text("Empty");
    } else {
        InventoryNode* current = inventory->getHead();
        while (current != nullptr) {
            std::string label = current->name + " (" + current->type + ") x" + std::to_string(current->quantity);
            if (ImGui::Button(label.c_str())) {
                if (current->type == "FOOD") stats.setHunger(stats.getHunger() - current->effect);
                else if (current->type == "HERB") stats.setHealth(stats.getHealth() + current->effect);
                inventory->useItem(current->name, current->effect, current->type);
            }
            current = current->next;
        }
    }

    ImGui::End();
}

// Single event window (floating)
void UI::displayEventGUI(const std::string& text) {
    ImGui::SetNextWindowSize(ImVec2(600, 80), ImGuiCond_Once);
    ImGui::Begin("Event", nullptr, ImGuiWindowFlags_NoCollapse);
    ImGui::TextWrapped("%s", text.c_str());
    ImGui::End();
}

// Event log (bottom-center)
void UI::displayEventLog(const std::vector<Event>& events) {
    ImGui::SetNextWindowPos(ImVec2(240, 420), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(800, 250), ImGuiCond_Once);
    ImGui::Begin("Event Log", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    for (const auto& e : events) {
        ImGui::TextWrapped("%s", e.getDescription().c_str());
    }

    ImGui::End();
}

// Welcome screen (centered)
bool UI::displayWelcomeScreen(bool& startGame) {
    ImGui::SetNextWindowPos(ImVec2(400, 200), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(480, 200), ImGuiCond_Once);
    ImGui::Begin("Welcome to Wolf Pack Survival", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::TextWrapped(
        "You are a lone wolf in the wilderness. Survive, hunt, and make strategic decisions to thrive."
    );
    ImGui::Spacing();
    if (ImGui::Button("Start Game", ImVec2(120, 40))) startGame = true;

    ImGui::End();
    return startGame;
}

// Ending screen (centered)
void UI::displayEndingGUI(const std::string& text) {
    ImGui::SetNextWindowPos(ImVec2(400, 200), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(480, 200), ImGuiCond_Once);
    ImGui::Begin("Game Over", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::TextWrapped("%s", text.c_str());
    ImGui::Spacing();
    ImGui::Text("Press ESC to close the game.");

    ImGui::End();
}
