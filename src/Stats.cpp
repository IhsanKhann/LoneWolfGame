#include "Stats.h"

// ============================================================
// CHANGES: Added validateStats() method as described in Listing 6.1
// ============================================================

Stats::Stats()
    : health(100),
      hunger(0),
      stamina(100),
      packStatus(50),
      morale(75),
      strength(50),
      xp(0) {}

// ---------------- Getters ----------------

int Stats::getHealth() const { return health; }
int Stats::getHunger() const { return hunger; }
int Stats::getStamina() const { return stamina; }
int Stats::getPackStatus() const { return packStatus; }

int Stats::getMorale() const { return morale; }
int Stats::getStrength() const { return strength; }
int Stats::getXP() const { return xp; }

// ---------------- Core Logic ----------------

void Stats::applyEffect(const StatEffect& effect) {
    health     += effect.healthChange;
    hunger     += effect.hungerChange;
    stamina    += effect.staminaChange;
    packStatus += effect.packStatusChange;

    morale     += effect.moraleChange;
    strength   += effect.strengthChange;
    xp         += effect.xpGain;

    clamp(health, 0, 100);
    clamp(hunger, 0, 100);
    clamp(stamina, 0, 100);
    clamp(packStatus, 0, 100);
    clamp(morale, 0, 100);
    clamp(strength, 0, 100);
}

void Stats::reset() {
    health = 100;
    hunger = 0;
    stamina = 100;
    packStatus = 50;
    morale = 75;
    strength = 50;
    xp = 0;
}

// Validate and apply penalties (as described in Listing 6.1)
void Stats::validateStats() {
    // Hunger penalty
    if (hunger > 100) {
        health -= 5;
    }
    
    // Stamina penalty
    if (stamina < 10) {
        morale -= 2;
    }
    
    // Morale penalty
    if (morale < 20) {
        stamina -= 3;
    }
    
    // Clamp after validation
    clamp(health, 0, 100);
    clamp(morale, 0, 100);
    clamp(stamina, 0, 100);
}

bool Stats::isDead() const {
    return health <= 0 || hunger >= 100 || morale <= 0;
}

bool Stats::canMakeChoice() const {
    return stamina >= 10 && morale >= 20;
}

// ---------------- Setters ----------------

void Stats::setHealth(int value) {
    health = value;
    clamp(health, 0, 100);
}

void Stats::setHunger(int value) {
    hunger = value;
    clamp(hunger, 0, 100);
}

void Stats::setStamina(int value) {
    stamina = value;
    clamp(stamina, 0, 100);
}

void Stats::setPackStatus(int value) {
    packStatus = value;
    clamp(packStatus, 0, 100);
}

void Stats::setMorale(int value) {
    morale = value;
    clamp(morale, 0, 100);
}

void Stats::setStrength(int value) {
    strength = value;
    clamp(strength, 0, 100);
}

void Stats::addXP(int value) {
    if (value > 0)
        xp += value;
}

// ---------------- Utility ----------------

void Stats::clamp(int& value, int min, int max) {
    if (value < min) value = min;
    if (value > max) value = max;
}