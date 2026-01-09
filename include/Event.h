#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <functional>

// Priority levels for event queue
enum class Priority {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3,
    CRITICAL = 4
};

// Stat changes that can be applied
struct StatEffect {
    int healthChange;
    int hungerChange;
    int staminaChange;
    int packStatusChange;
    int moraleChange;
    int strengthChange;
    int xpGain;
    
    StatEffect(int h = 0, int hu = 0, int st = 0, int p = 0, 
               int m = 0, int str = 0, int xp = 0);
    
    // Return inverse effect for undo
    StatEffect reverse() const;
};

// Event class with priority and effects
class Event {
public:
    Event(int id = 0,
          const std::string& description = "",
          Priority priority = Priority::MEDIUM,
          const StatEffect& effect = StatEffect());
    
    int getId() const;
    std::string getDescription() const;
    Priority getPriority() const;
    const StatEffect& getEffect() const;
    
    // Execute custom action
    virtual void execute();
    void setAction(std::function<void()> act);
    
    // Comparison for priority queue
    bool operator<(const Event& other) const;

private:
    int id;
    std::string description;
    Priority priority;
    StatEffect effect;
    std::function<void()> action;
};

#endif