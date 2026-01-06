#include "Stats.h"

Stats::Stats() : health(100), hunger(0), stamina(100), packStatus(50) {}

int Stats::getHealth() const {
    return health;
}

int Stats::getHunger() const {
    return hunger;
}

int Stats::getStamina() const {
    return stamina;
}

int Stats::getPackStatus() const {
    return packStatus;
}

void Stats::applyEffect(const StatEffect& effect) {
    health += effect.healthChange;
    hunger += effect.hungerChange;
    stamina += effect.staminaChange;
    packStatus += effect.packStatusChange;
    
    clamp(health, 0, 100);
    clamp(hunger, 0, 100);
    clamp(stamina, 0, 100);
    clamp(packStatus, 0, 100);
}

void Stats::reset() {
    health = 100;
    hunger = 0;
    stamina = 100;
    packStatus = 50;
}

bool Stats::isDead() const {
    return health <= 0 || hunger >= 100;
}

bool Stats::canMakeChoice() const {
    return stamina >= 10;
}

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

void Stats::clamp(int& value, int min, int max) {
    if (value < min) value = min;
    if (value > max) value = max;
}