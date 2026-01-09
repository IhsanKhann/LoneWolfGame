#ifndef STATS_H
#define STATS_H

#include "Event.h"

class Stats {
public:
    Stats();

    // Core stats
    int getHealth() const;
    int getHunger() const;
    int getStamina() const;
    int getPackStatus() const;

    // Extended stats
    int getMorale() const;
    int getStrength() const;
    int getXP() const;

    void applyEffect(const StatEffect& effect);
    void reset();
    bool isDead() const;
    bool canMakeChoice() const;
    
    // Validate and clamp stats (as described in Listing 6.1)
    void validateStats();

    // Setters
    void setHealth(int value);
    void setHunger(int value);
    void setStamina(int value);
    void setPackStatus(int value);
    void setMorale(int value);
    void setStrength(int value);
    void addXP(int value);

private:
    int health;
    int hunger;
    int stamina;
    int packStatus;

    int morale;
    int strength;
    int xp;

    void clamp(int& value, int min, int max);
};

#endif