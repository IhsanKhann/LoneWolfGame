#include "../include/UI.h"
#include <imgui.h>
#include <iostream>

// Display stats in a panel
void UI::displayStatsGUI(const Stats& stats) {
    ImGui::Begin("Stats");
    ImGui::Text("Health: %d", stats.getHealth());
    ImGui::Text("Hunger: %d", stats.getHunger());
    ImGui::Text("Stamina: %d", stats.getStamina());
    ImGui::Text("Pack Status: %d", stats.getPackStatus());
    ImGui::End();
}

// Display an event
void UI::displayEventGUI(const std::string& text) {
    ImGui::Begin("Event");
    ImGui::TextWrapped("%s", text.c_str());
    ImGui::End();
}

// Display node text and choices, return choice via selectedChoice
void UI::displayNodeGUI(const Node* node, int& selectedChoice) {
    if (!node) return;

    ImGui::Begin("Decision Node");
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

// Display inventory with buttons to use items
void UI::showInventoryGUI(Inventory* inventory, Stats& stats) {
    ImGui::Begin("Inventory");

    if (inventory->getSize() == 0) {
        ImGui::Text("Empty");
    } else {
        InventoryNode* current = inventory->getHead();
        while (current != nullptr) {
            std::string buttonLabel = current->name + " (" + current->type + ") x" + std::to_string(current->quantity);
            if (ImGui::Button(buttonLabel.c_str())) {
                // Apply effect when clicked
                if (current->type == "FOOD") {
                    stats.setHunger(stats.getHunger() - current->effect);
                } else if (current->type == "HERB") {
                    stats.setHealth(stats.getHealth() + current->effect);
                }
                inventory->useItem(current->name, current->effect, current->type);
            }
            current = current->next;
        }
    }

    ImGui::End();
}

// Display ending window
void UI::displayEndingGUI(const std::string& text) {
    ImGui::Begin("Game Over");
    ImGui::TextWrapped("%s", text.c_str());
    ImGui::Text("Press ESC to close the game.");
    ImGui::End();
}
