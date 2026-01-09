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
// UPDATED:
// - Added ESC key handling to properly close the window
// - Visual feedback for undo actions
// - Integrated UI undo controls with backend
// - Confirmation messages for all actions
// - Proper state restoration with UI sync
// ============================================================

// Notification system for user feedback
struct Notification {
    std::string message;
    float timeRemaining;
    ImVec4 color;
    
    Notification(const std::string& msg, float duration = 3.0f, ImVec4 col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f))
        : message(msg), timeRemaining(duration), color(col) {}
};

std::vector<Notification> notifications;

void addNotification(const std::string& message, ImVec4 color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f)) {
    notifications.push_back(Notification(message, 3.0f, color));
}

void updateAndRenderNotifications(float deltaTime) {
    ImGui::SetNextWindowPos(ImVec2(400, 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(480, 100), ImGuiCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
    
    if (!notifications.empty()) {
        ImGui::Begin("##Notifications", nullptr, 
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
        
        for (auto it = notifications.begin(); it != notifications.end();) {
            it->timeRemaining -= deltaTime;
            
            if (it->timeRemaining <= 0) {
                it = notifications.erase(it);
            } else {
                float alpha = std::min(1.0f, it->timeRemaining);
                ImVec4 color = it->color;
                color.w = alpha;
                ImGui::TextColored(color, "%s", it->message.c_str());
                ++it;
            }
        }
        
        ImGui::End();
    }
    
    ImGui::PopStyleVar();
}

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
        addNotification("Found: Winter Berries!", ImVec4(0.5f, 1.0f, 0.5f, 1.0f));
    }
    else if (roll <= 25 && !inventory->isFull()) {
        inventory->addItem("Healing Herbs", "HERB", 20, 1);
        addNotification("Found: Healing Herbs!", ImVec4(0.5f, 1.0f, 0.5f, 1.0f));
    }
    else if (roll <= 35) {
        eventManager.registerEvent(101, "A harsh wind chills you to the bone.", Priority::MEDIUM, StatEffect(0, 5, -15, 0));
        eventManager.triggerEvent(101);
        addNotification("A harsh wind strikes!", ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
    }
    else if (roll <= 40 && !inventory->isFull()) {
        inventory->addItem("Dried Meat", "FOOD", 25, 1);
        addNotification("Found: Dried Meat!", ImVec4(0.5f, 1.0f, 0.5f, 1.0f));
    }
}

// ---------------- Game Loop ----------------
void gameLoop(
    GLFWwindow* window,
    DecisionTree& tree,
    EventManager& em,
    GameState& gameState,
    GameStateStack& stateHistory,
    ActionQueue& actionQueue,
    std::vector<Event>& eventLog,
    float deltaTime
) {
    int selectedChoice = -1;
    const Node* node = tree.getCurrentNode();
    if (!node) return;

    // Check for ESC key to close the game
    UIManager::checkEscapeKey(window);

    // Render UI using UIManager with undo controls
    bool undoRequested = false;
    bool clearHistoryRequested = false;
    
    UIManager::render(gameState, tree, eventLog, selectedChoice, stateHistory, actionQueue);
    
    // Handle undo controls from UI
    UIManager::displayActionControls(stateHistory, actionQueue, undoRequested, clearHistoryRequested);
    
    // Handle undo request (from button or U key)
    if (undoRequested || ImGui::IsKeyPressed(ImGuiKey_U)) {
        GameState previousState;
        if (stateHistory.undo(previousState)) {
            // Restore complete game state
            gameState = previousState;
            tree.setCurrentNode(gameState.currentNodeId);
            
            addNotification("⟲ Restored previous state", ImVec4(0.5f, 0.5f, 1.0f, 1.0f));
            std::cout << "UNDO: Restored to Node " << gameState.currentNodeId 
                      << " (Day " << gameState.day << ")" << std::endl;
        } else {
            addNotification("Cannot undo - no history!", ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
        }
    }
    
    // Handle clear history request
    if (clearHistoryRequested) {
        stateHistory.clear();
        actionQueue.clear();
        addNotification("History cleared!", ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
        std::cout << "Cleared all undo history" << std::endl;
    }

    // Handle choice selection
    if (selectedChoice != -1) {
        // Save current state for undo BEFORE making changes
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
            
            // Visual feedback
            addNotification("Choice made: " + choice.text.substr(0, 40) + "...", 
                          ImVec4(0.8f, 0.8f, 1.0f, 1.0f));
            
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
            
            std::cout << "DAY " << gameState.day << ": Moved to Node " 
                      << gameState.currentNodeId << std::endl;
        }
    }
    
    // Process high-priority events (Algorithm 2)
    if (em.hasEvents()) {
        em.update(&gameState.stats, [&eventLog](const std::string& msg) {
            Event notification(999, msg, Priority::HIGH, StatEffect());
            eventLog.push_back(notification);
            addNotification(msg, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));
        });
    }
    
    // Check for ending
    if (node->isEndingNode() || gameState.stats.isDead()) {
        std::string endMessage = node->isEndingNode() ? 
            node->getEndingType() : 
            "You have perished in the wilderness...";
        UIManager::displayEndingGUI(endMessage);
    }
    
    // Update and render notifications
    updateAndRenderNotifications(deltaTime);
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
    
    // Delta time tracking
    float lastFrameTime = static_cast<float>(glfwGetTime());

    tree.loadNodes();
    initializeEvents(em);

    std::cout << "=== Wolf Pack Survival ===" << std::endl;
    std::cout << "Press U to undo your last choice" << std::endl;
    std::cout << "Press ESC to quit" << std::endl;
    std::cout << "=========================" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        float currentFrameTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (!startGame) {
            // Check for ESC on welcome screen too
            UIManager::checkEscapeKey(window);
            
            UIManager::displayWelcomeScreen(startGame);
            if (startGame) {
                addNotification("Game started! Good luck, lone wolf.", 
                              ImVec4(0.5f, 1.0f, 0.5f, 1.0f));
            }
        } else {
            gameLoop(window, tree, em, gameState, history, actionQueue, eventLog, deltaTime);
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

    std::cout << "\n=== Game Statistics ===" << std::endl;
    std::cout << "Days Survived: " << gameState.day << std::endl;
    std::cout << "Final XP: " << gameState.stats.getXP() << std::endl;
    std::cout << "Undo History Size: " << history.getSize() << std::endl;
    std::cout << "Game closed via ESC key." << std::endl;
    std::cout << "=======================" << std::endl;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    delete gameState.inventory;

    return 0;
}