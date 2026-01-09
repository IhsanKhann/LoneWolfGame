#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Event.h"
#include "Stats.h"
#include <map>
#include <queue>
#include <functional>

class EventManager {
public:
    EventManager();

    // Register an event template
    bool registerEvent(int id,
                       const std::string& description,
                       Priority priority,
                       const StatEffect& effect);

    // Trigger a registered event (adds to queue)
    bool triggerEvent(int eventId);
    
    // Push event directly to queue (as described in Listing 5.1)
    void pushEvent(Priority p, const std::string& msg, std::function<void()> action);

    bool hasEvents() const;

    // Get and execute highest-priority event (implements Algorithm 2)
    Event getNextEvent();
    
    // Update method - processes events with threshold and notification (Algorithm 2)
    void update(Stats* stats, std::function<void(const std::string&)> notifyUI);
    
    // Stat polling for automatic event triggering (Ch 5.2)
    void pollStats(Stats* stats);

    void clear();

private:
    std::map<int, Event> eventRegistry;
    std::priority_queue<Event> eventQueue;
    
    static const int CRITICAL_THRESHOLD = 20; // For Algorithm 2
};

#endif