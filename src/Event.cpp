#include "Event.h"

StatEffect::StatEffect(int h, int hu, int st, int p)
    : healthChange(h), hungerChange(hu), staminaChange(st), packStatusChange(p) {}

Event::Event(int id, const std::string& description, Priority priority, const StatEffect& effect)
    : id(id), description(description), priority(priority), effect(effect) {}

int Event::getId() const {
    return id;
}

std::string Event::getDescription() const {
    return description;
}

Priority Event::getPriority() const {
    return priority;
}

const StatEffect& Event::getEffect() const {
    return effect;
}

bool Event::operator<(const Event& other) const {
    return static_cast<int>(priority) > static_cast<int>(other.priority);
}