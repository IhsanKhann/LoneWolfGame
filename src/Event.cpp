#include "Event.h"

// ============================================================
// StatEffect Implementation
// ============================================================

StatEffect::StatEffect(
    int h,
    int hu,
    int st,
    int p,
    int m,
    int str,
    int xp
)
    : healthChange(h),
      hungerChange(hu),
      staminaChange(st),
      packStatusChange(p),
      moraleChange(m),
      strengthChange(str),
      xpGain(xp) {}

StatEffect StatEffect::reverse() const {
    return StatEffect(
        -healthChange,
        -hungerChange,
        -staminaChange,
        -packStatusChange,
        -moraleChange,
        -strengthChange,
        -xpGain
    );
}

// ============================================================
// Event Implementation
// ============================================================

Event::Event(
    int id,
    const std::string& description,
    Priority priority,
    const StatEffect& effect
)
    : id(id),
      description(description),
      priority(priority),
      effect(effect),
      action(nullptr) {}

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

void Event::execute() {
    // Virtual method - executes custom action if set
    if (action) {
        action();
    }
}

void Event::setAction(std::function<void()> act) {
    action = act;
}

// Higher priority comes first in priority_queue
bool Event::operator<(const Event& other) const {
    return static_cast<int>(priority) < static_cast<int>(other.priority);
}