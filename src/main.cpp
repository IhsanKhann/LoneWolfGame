#include "../include/DecisionTree.h"
#include "../include/EventManager.h"
#include "../include/Stats.h"
#include "../include/UI.h"
#include "../include/Inventory.h"
#include "../include/GameState.h"
#include "../include/ActionQueue.h"

#include <iostream>
#include <vector>
#include <random>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Initialize all game events
void initializeEvents(EventManager& eventManager) {
    eventManager.registerEvent(1, "The cold wind bites at your fur. Your body shivers.", Priority::NORMAL, StatEffect(0,5,-10,0));
    eventManager.registerEvent(2, "The icy air drains your energy as you track the prey.", Priority::NORMAL, StatEffect(0,5,-15,0));
    eventManager.registerEvent(3, "Your hunger grows as you wait in the snow.", Priority::NORMAL, StatEffect(0,10,-5,0));
    eventManager.registerEvent(4, "The ice cracks dangerously beneath you!", Priority::HIGH, StatEffect(-20,0,-20,0));
    eventManager.registerEvent(5, "The long detour exhausts you further.", Priority::NORMAL, StatEffect(0,10,-15,0));
    eventManager.registerEvent(6, "Tension rises as the strange wolves approach.", Priority::NORMAL, StatEffect(0,5,-10,5));
    eventManager.registerEvent(7, "The encounter leaves you wary and alert.", Priority::NORMAL, StatEffect(0,5,-10,0));
    eventManager.registerEvent(8, "You feast on fresh venison! Your strength returns.", Priority::HIGH, StatEffect(30,-50,40,0));
    eventManager.registerEvent(9, "Caution preserves your energy.", Priority::LOW, StatEffect(0,5,5,0));
    eventManager.registerEvent(10, "Ancient knowledge fills you with confidence.", Priority::LOW, StatEffect(10,0,0,10));
}

// Random event generator
void generateRandomEvent(EventManager& eventManager, Inventory* inventory) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1,100);
    int roll = dis(gen);

    if (roll <= 15) {
        eventManager.registerEvent(100, "You found some winter berries hidden under snow!", Priority::LOW, StatEffect(0,-10,0,0));
        eventManager.triggerEvent(100);
        inventory->addItem("Winter Berries", "FOOD", 10, 1);
    } else if (roll <= 25 && !inventory->isFull()) {
        inventory->addItem("Healing Herbs", "HERB", 20, 1);
    } else if (roll <= 35) {
        eventManager.registerEvent(101, "A harsh wind chills you to the bone.", Priority::NORMAL, StatEffect(0,5,-15,0));
        eventManager.triggerEvent(101);
    } else if (roll <= 40 && !inventory->isFull()) {
        inventory->addItem("Dried Meat", "FOOD", 25, 1);
    }
}

// Game loop
void gameLoop(DecisionTree& tree, EventManager& em, Stats& stats, Inventory* inventory, 
              GameStateStack& stateHistory, ActionQueue& queue, int& day, int& packSize,
              std::vector<Event>& eventLog) {

    int selectedChoice = -1;
    const Node* node = tree.getCurrentNode();
    if (!node) return;

    // Display fixed panels
    UI::displayStatsPanel(stats, day, packSize, "Winter");
    UI::displayNodeGUI(node, selectedChoice);
    UI::showInventoryGUI(inventory, stats);
    UI::displayEventLog(eventLog);

    if (selectedChoice != -1) {
        stateHistory.push(node->getId(), stats, inventory, day, packSize);
        tree.makeChoice(selectedChoice);
        day++;
        stats.setHunger(stats.getHunger() + 5);
        stats.setStamina(stats.getStamina() - 10);
        generateRandomEvent(em, inventory);
    }
}

// Main
int main() {
    // Initialize GLFW
    if (!glfwInit()) return 1;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Wolf Pack Survival", NULL, NULL);
    if (!window) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable VSync

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Scale UI for readability
    io.FontGlobalScale = 1.5f;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Game objects
    DecisionTree tree;
    EventManager em;
    Stats stats;
    Inventory* inventory = new Inventory();
    GameStateStack history;
    ActionQueue queue;
    std::vector<Event> eventLog;
    int day = 1, packSize = 1;
    bool startGame = false;

    tree.loadNodes();
    initializeEvents(em);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Fixed layout with central welcome screen
        if (!startGame) {
            UI::displayWelcomeScreen(startGame);
        } else {
            gameLoop(tree, em, stats, inventory, history, queue, day, packSize, eventLog);
        }

        // Render ImGui
        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
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
