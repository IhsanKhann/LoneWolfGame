#include "EventManager.h"

EventManager::EventManager() {}

void EventManager::registerEvent(int id, const std::string& description, Priority priority, const StatEffect& effect) {
    eventRegistry.emplace(id, Event(id, description, priority, effect));
}

void EventManager::triggerEvent(int eventId) {
    auto it = eventRegistry.find(eventId);
    if (it != eventRegistry.end()) {
        eventQueue.push(it->second);
    }
}

bool EventManager::hasEvents() const {
    return !eventQueue.empty();
}

Event EventManager::getNextEvent() {
    Event evt = eventQueue.top();
    eventQueue.pop();
    return evt;
}

void EventManager::clear() {
    while (!eventQueue.empty()) {
        eventQueue.pop();
    }
}
