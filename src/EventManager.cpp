#include "EventManager.h"
#include <iostream>

// ============================================================
// CHANGES: Implemented Algorithm 2 (Priority Event Dispatcher)
// Added update() method with threshold and notification
// Added pushEvent() as described in Listing 5.1
// Added pollStats() for automatic stat-based event triggering (Ch 5.2)
// ============================================================

EventManager::EventManager() = default;

// ---------------- Registration ----------------

bool EventManager::registerEvent(int id,
                                 const std::string& description,
                                 Priority priority,
                                 const StatEffect& effect) {
    // Prevent duplicate IDs
    if (eventRegistry.find(id) != eventRegistry.end())
        return false;

    eventRegistry.emplace(id, Event(id, description, priority, effect));
    return true;
}

// ---------------- Triggering ----------------

bool EventManager::triggerEvent(int eventId) {
    auto it = eventRegistry.find(eventId);
    if (it == eventRegistry.end())
        return false;

    eventQueue.push(it->second);
    return true;
}

void EventManager::pushEvent(Priority p, const std::string& msg, std::function<void()> action) {
    // Create event with priority and action
    Event evt(0, msg, p, StatEffect());
    evt.setAction(action);
    eventQueue.push(evt);
}

// ---------------- Queue Management ----------------

bool EventManager::hasEvents() const {
    return !eventQueue.empty();
}

Event EventManager::getNextEvent() {
    Event evt = eventQueue.top();
    eventQueue.pop();
    return evt;
}

// Algorithm 2: Priority Event Dispatcher implementation
void EventManager::update(Stats* stats, std::function<void(const std::string&)> notifyUI) {
    if (!hasEvents()) return;
    
    Event e = getNextEvent();
    
    // Check threshold for critical events (Algorithm 2)
    if (static_cast<int>(e.getPriority()) >= static_cast<int>(Priority::HIGH)) {
        // Execute effect
        e.execute();
        if (stats) {
            stats->applyEffect(e.getEffect());
        }
        
        // Notify UI
        if (notifyUI) {
            notifyUI(e.getDescription());
        }
    }
}

// Stat polling mechanism (Ch 5.2) - automatically triggers events based on stat values
void EventManager::pollStats(Stats* stats) {
    if (!stats) return;
    
    // Check for critical stat conditions
    if (stats->getHealth() < CRITICAL_THRESHOLD) {
        pushEvent(Priority::CRITICAL, "Your health is critically low!", [stats]() {
            std::cout << "CRITICAL: Low health detected!" << std::endl;
        });
    }
    
    if (stats->getHunger() > 80) {
        pushEvent(Priority::HIGH, "You are starving!", [stats]() {
            stats->setHealth(stats->getHealth() - 5);
        });
    }
    
    if (stats->getStamina() < CRITICAL_THRESHOLD) {
        pushEvent(Priority::MEDIUM, "Exhaustion overwhelms you.", [stats]() {
            std::cout << "WARNING: Low stamina!" << std::endl;
        });
    }
    
    if (stats->getMorale() < 10) {
        pushEvent(Priority::HIGH, "Despair sets in...", [stats]() {
            std::cout << "WARNING: Morale critical!" << std::endl;
        });
    }
}

void EventManager::clear() {
    while (!eventQueue.empty())
        eventQueue.pop();
}