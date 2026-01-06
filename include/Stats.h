#ifndef STATS_H
#define STATS_H

#include "Event.h"

class Stats {
public:
    Stats();
    
    int getHealth() const;
    int getHunger() const;
    int getStamina() const;
    int getPackStatus() const;
    
    void applyEffect(const StatEffect& effect);
    void reset();
    bool isDead() const;
    bool canMakeChoice() const;
    
    // New methods for complete game mechanics
    void setHealth(int value);
    void setHunger(int value);
    void setStamina(int value);
    void setPackStatus(int value);

private:
    int health;
    int hunger;
    int stamina;
    int packStatus;
    
    void clamp(int& value, int min, int max);
};

#endif