#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Event.h"
#include <map>
#include <queue>
#include <vector>

class EventManager {
public:
    EventManager();
    
    void registerEvent(int id, const std::string& description, Priority priority, const StatEffect& effect);
    void triggerEvent(int eventId);
    bool hasEvents() const;
    Event getNextEvent();
    void clear();

private:
    std::map<int, Event> eventRegistry;
    std::priority_queue<Event> eventQueue;
};

#endif