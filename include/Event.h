#ifndef EVENT_H
#define EVENT_H

#include <string>

enum class Priority {
    LOW = 3,
    NORMAL = 2,
    HIGH = 1
};

struct StatEffect {
    int healthChange;
    int hungerChange;
    int staminaChange;
    int packStatusChange;
    
    StatEffect(int h = 0, int hu = 0, int st = 0, int p = 0);
};

class Event {
public:
    Event(int id, const std::string& description, Priority priority, const StatEffect& effect);
    
    int getId() const;
    std::string getDescription() const;
    Priority getPriority() const;
    const StatEffect& getEffect() const;
    
    bool operator<(const Event& other) const;

private:
    int id;
    std::string description;
    Priority priority;
    StatEffect effect;
};

#endif