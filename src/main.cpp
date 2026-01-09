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

// ============================================================
// CHANGES:
// - Integrated ActionQueue with Command pattern for undo
// - Implemented stat polling via EventManager
// - Applied choice-based stat effects
// - Added undo functionality (U key)
// - Integrated Algorithm 2 (event update with threshold)
// - Used GameState struct as centralized state holder
// - Implemented validateStats() calls
// ============================================================

// ---------------- Initialize Events ----------------
void initializeEvents(EventManager& eventManager) {
    eventManager.registerEvent(1, "The cold wind bites at your fur. Your body shivers.", Priority::MEDIUM, StatEffect(0, 5, -10, 0));
    eventManager.registerEvent(2, "The icy air drains your energy as you track the prey.", Priority::MEDIUM, StatEffect(0, 5, -15, 0));
    eventManager.registerEvent(3, "Your hunger grows as you wait in the snow.", Priority::MEDIUM, StatEffect(0, 10, -5, 0));
    eventManager.registerEvent(4, "The ice cracks dangerously beneath you!", Priority::HIGH, StatEffect(-20, 0, -20, 0));
    eventManager.registerEvent(5, "The long detour exhausts you further.", Priority::MEDIUM, StatEffect(0, 10, -15, 0));
    eventManager.registerEvent(6, "Tension rises as the strange wolves approach.", Priority::MEDIUM, StatEffect(0, 5, -10, 5));
    eventManager.registerEvent(7, "The encounter leaves you wary and alert.", Priority::MEDIUM, StatEffect(0, 5, -10, 0));
    eventManager.registerEvent(8, "You feast on fresh venison! Your strength returns.", Priority::HIGH, StatEffect(30, -50, 40, 0));
    eventManager.registerEvent(9, "Caution preserves your energy.", Priority::LOW, StatEffect(0, 5, 5, 0));
    eventManager.registerEvent(10, "Ancient knowledge fills you with confidence.", Priority::LOW, StatEffect(10, 0, 0, 10));
}

// ---------------- Random Events ----------------
void generateRandomEvent(EventManager& eventManager, Inventory* inventory) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    int roll = dis(gen);

    if (roll <= 15) {
        eventManager.registerEvent(100, "You found some winter berries hidden under snow!", Priority::LOW, StatEffect(0, -10, 0, 0));
        eventManager.triggerEvent(100);
        inventory->addItem("Winter Berries", "FOOD", 10, 1);
    }
    else if (roll <= 25 && !inventory->isFull()) {
        inventory->addItem("Healing Herbs", "HERB", 20, 1);
    }
    else if (roll <= 35) {
        eventManager.registerEvent(101, "A harsh wind chills you to the bone.", Priority::MEDIUM, StatEffect(0, 5, -15, 0));
        eventManager.triggerEvent(101);
    }
    else if (roll <= 40 && !inventory->isFull()) {
        inventory->addItem("Dried Meat", "FOOD", 25, 1);
    }
}

// ---------------- Game Loop ----------------
void gameLoop(
    DecisionTree& tree,
    EventManager& em,
    GameState& gameState,
    GameStateStack& stateHistory,
    ActionQueue& actionQueue,
    std::vector<Event>& eventLog
) {
    int selectedChoice = -1;
    const Node* node = tree.getCurrentNode();
    if (!node) return;

    // Render UI using UIManager
    UIManager::render(gameState, tree, eventLog, selectedChoice);

    // Handle choice selection
    if (selectedChoice != -1) {
        // Save current state for undo
        stateHistory.push(node->getId(), gameState.stats, gameState.inventory, gameState.day, gameState.packSize);
        
        // Get choice with effects
        const auto& choices = node->getChoicesWithEffects();
        if (selectedChoice >= 0 && selectedChoice < static_cast<int>(choices.size())) {
            const Choice& choice = choices[selectedChoice];
            
            // Apply choice effects using Command pattern
            for (const auto& effect : choice.effects) {
                Command* cmd = new StatChangeCommand(&gameState.stats, effect, choice.text);
                actionQueue.executeAndTrack(cmd, "Choice Effect", choice.text);
            }
            
            // Navigate to next node
            tree.makeChoice(selectedChoice);
            gameState.currentNodeId = tree.getCurrentNode()->getId();
            
            // Advance day and apply passive effects
            gameState.day++;
            gameState.stats.setHunger(gameState.stats.getHunger() + 5);
            gameState.stats.setStamina(gameState.stats.getStamina() - 10);
            gameState.stats.validateStats();
            
            // Trigger node events
            const Node* newNode = tree.getCurrentNode();
            if (newNode) {
                for (int eventId : newNode->getTriggers()) {
                    if (em.triggerEvent(eventId)) {
                        Event evt = em.getNextEvent();
                        gameState.stats.applyEffect(evt.getEffect());
                        eventLog.push_back(evt);
                    }
                }
            }
            
            // Generate random events
            generateRandomEvent(em, gameState.inventory);
            
            // Poll stats for critical events (Algorithm 2, Ch 5.2)
            em.pollStats(&gameState.stats);
        }
    }
    
    // Process high-priority events (Algorithm 2)
    if (em.hasEvents()) {
        em.update(&gameState.stats, [&eventLog](const std::string& msg) {
            Event notification(999, msg, Priority::HIGH, StatEffect());
            eventLog.push_back(notification);
        });
    }
    
    // Check for undo input (U key)
    if (ImGui::IsKeyPressed(ImGuiKey_U)) {
        GameState previousState;
        if (stateHistory.undo(previousState)) {
            gameState = previousState;
            tree.setCurrentNode(gameState.currentNodeId);
            std::cout << "Undo: Restored previous state" << std::endl;
        }
    }
    
    // Check for ending
    if (node->isEndingNode() || gameState.stats.isDead()) {
        std::string endMessage = node->isEndingNode() ? 
            node->getEndingType() : 
            "You have perished in the wilderness...";
        UIManager::displayEndingGUI(endMessage);
    }
}

// ---------------- Main ----------------
int main() {
    if (!glfwInit()) return 1;

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Wolf Pack Survival", nullptr, nullptr);
    if (!window) return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.5f;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Initialize game components
    DecisionTree tree;
    EventManager em;
    GameState gameState;  // Centralized state as described in Ch 6.3
    gameState.inventory = new Inventory();
    
    GameStateStack history;
    ActionQueue actionQueue;  // Now properly integrated with Command pattern
    std::vector<Event> eventLog;

    bool startGame = false;

    tree.loadNodes();
    initializeEvents(em);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (!startGame) {
            UIManager::displayWelcomeScreen(startGame);
        } else {
            gameLoop(tree, em, gameState, history, actionQueue, eventLog);
        }

        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    delete gameState.inventory;

    return 0;
}