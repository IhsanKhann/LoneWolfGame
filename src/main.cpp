#include "../include/DecisionTree.h"
#include "../include/EventManager.h"
#include "../include/Stats.h"
#include "../include/UI.h"
#include "../include/Inventory.h"
#include "../include/GameState.h"
#include "../include/ActionQueue.h"

#include <iostream>
#include <fstream>
#include <random>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Initialize all game events
void initializeEvents(EventManager& eventManager) {
    eventManager.registerEvent(1, "The cold wind bites at your fur. Your body shivers.", Priority::NORMAL, StatEffect(0, 5, -10, 0));
    eventManager.registerEvent(2, "The icy air drains your energy as you track the prey.", Priority::NORMAL, StatEffect(0, 5, -15, 0));
    eventManager.registerEvent(3, "Your hunger grows as you wait in the snow.", Priority::NORMAL, StatEffect(0, 10, -5, 0));
    eventManager.registerEvent(4, "The ice cracks dangerously beneath you!", Priority::HIGH, StatEffect(-20, 0, -20, 0));
    eventManager.registerEvent(5, "The long detour exhausts you further.", Priority::NORMAL, StatEffect(0, 10, -15, 0));
    eventManager.registerEvent(6, "Tension rises as the strange wolves approach.", Priority::NORMAL, StatEffect(0, 5, -10, 5));
    eventManager.registerEvent(7, "The encounter leaves you wary and alert.", Priority::NORMAL, StatEffect(0, 5, -10, 0));
    eventManager.registerEvent(8, "You feast on fresh venison! Your strength returns.", Priority::HIGH, StatEffect(30, -50, 40, 0));
    eventManager.registerEvent(9, "Caution preserves your energy.", Priority::LOW, StatEffect(0, 5, 5, 0));
    eventManager.registerEvent(10, "Ancient knowledge fills you with confidence.", Priority::LOW, StatEffect(10, 0, 0, 10));
    // ... add other events as needed
}

// Random event generator
void generateRandomEvent(EventManager& eventManager, Inventory* inventory) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    int roll = dis(gen);

    if (roll <= 15) {
        eventManager.registerEvent(100, "You found some winter berries hidden under snow!", Priority::LOW, StatEffect(0, -10, 0, 0));
        eventManager.triggerEvent(100);
        inventory->addItem("Winter Berries", "FOOD", 10, 1);
    } else if (roll <= 25) {
        if (!inventory->isFull()) {
            inventory->addItem("Healing Herbs", "HERB", 20, 1);
        }
    } else if (roll <= 35) {
        eventManager.registerEvent(101, "A harsh wind chills you to the bone.", Priority::NORMAL, StatEffect(0, 5, -15, 0));
        eventManager.triggerEvent(101);
    } else if (roll <= 40) {
        if (!inventory->isFull()) {
            inventory->addItem("Dried Meat", "FOOD", 25, 1);
        }
    }
}

// Main game loop integrated with ImGui
void gameLoopGUI(DecisionTree& tree, EventManager& eventManager, Stats& stats, Inventory* inventory, 
                 GameStateStack& stateHistory, ActionQueue& actionQueue, int& day, int& packSize) {

    int selectedChoice = -1;

    while (true) {
        const Node* currentNode = tree.getCurrentNode();
        if (!currentNode) break;

        // Process queued actions
        if (!actionQueue.isEmpty()) {
            std::string actionName, actionDesc;
            if (actionQueue.dequeue(actionName, actionDesc)) {
                ImGui::Begin("Action");
                ImGui::TextWrapped("%s", actionDesc.c_str());
                ImGui::End();
            }
        }

        // Trigger node events
        for (int eventId : currentNode->getTriggers()) {
            eventManager.triggerEvent(eventId);
        }

        // Process events
        while (eventManager.hasEvents()) {
            Event evt = eventManager.getNextEvent();
            UI::displayEventGUI(evt.getDescription());
            stats.applyEffect(evt.getEffect());
        }

        // Check for death
        if (stats.isDead()) {
            if (stats.getHealth() <= 0) {
                UI::displayEndingGUI("Your wounds proved fatal. You collapse in the snow, your journey ending here in the wilderness.");
            } else if (stats.getHunger() >= 100) {
                UI::displayEndingGUI("Starvation claims you. Weak and unable to hunt, you fade away in the frozen landscape.");
            }
            break;
        }

        // Check for ending node
        if (currentNode->isEndingNode()) {
            std::string endingText = currentNode->getText();
            if (!currentNode->getEndingType().empty()) {
                endingText += "\n\n=== " + currentNode->getEndingType() + " ===";
            }
            endingText += "\n\nYou survived " + std::to_string(day) + " days in the wilderness.";
            UI::displayEndingGUI(endingText);
            break;
        }

        // Display stats panel
        UI::displayStatsGUI(stats);

        // Display node text and choices
        selectedChoice = -1;
        UI::displayNodeGUI(currentNode, selectedChoice);

        // Display inventory panel
        UI::showInventoryGUI(inventory, stats);

        // If a choice is made
        if (selectedChoice != -1) {
            // Save current state
            stateHistory.push(currentNode->getId(), stats, inventory, day, packSize);

            // Make the choice
            tree.makeChoice(selectedChoice);

            // Update stats and day
            day++;
            stats.setHunger(stats.getHunger() + 5);
            stats.setStamina(stats.getStamina() - 10);

            // Generate random events
            generateRandomEvent(eventManager, inventory);
        }

        // Break out of loop if no node or user exits
        if (!tree.getCurrentNode()) break;

        // Render GUI frame
        ImGui::Render();
    }
}

int main() {
    // Setup GLFW
    if (!glfwInit()) return 1;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Wolf Pack Survival", NULL, NULL);
    if (!window) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Game objects
    DecisionTree tree;
    EventManager eventManager;
    Stats stats;
    Inventory* inventory = new Inventory();
    GameStateStack stateHistory;
    ActionQueue actionQueue;
    int day = 1;
    int packSize = 1;

    tree.loadNodes();
    initializeEvents(eventManager);

    bool showWelcome = true;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (showWelcome) {
            ImGui::Begin("Wolf Pack Survival");
            ImGui::TextWrapped("Welcome to Wolf Pack Survival!\nYour goal is to survive in the wilderness and make strategic decisions.");
            if (ImGui::Button("Start Game")) {
                showWelcome = false;
                tree.reset();
                stats.reset();
                inventory->clear();
                stateHistory.clear();
                actionQueue.clear();
                day = 1;
                packSize = 1;
            }
            ImGui::End();
        } else {
            gameLoopGUI(tree, eventManager, stats, inventory, stateHistory, actionQueue, day, packSize);
        }

        // Render ImGui frame
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    delete inventory;

    return 0;
}
