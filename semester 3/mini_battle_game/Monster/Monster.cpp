#include "Monster.h"
#include "../Player/Player.h"
#include <iostream>

Monster::Monster() : Monster("Default_Monster", 20, 100) {}

Monster::Monster(const std::string& name, int power, int max_health)
    : name(name),
      current_health(max_health),
      max_health(max_health),
      power(power) {
    if (max_health <= 0) {
        this->max_health = 100;
    }
    if (power <= 0) {
        this->power = 20;
    }
    current_health = this->max_health;
}

Monster::~Monster() {}

void Monster::takeDamage(int damage) {
    if (damage < 0) {
        damage = 0;
    }
    current_health -= damage;
    if (current_health < 0) {
        current_health = 0;
    }
}

void Monster::attackPlayer(Player& player)const {
    player.takeDamage(power);
}

bool Monster::isAlive() const {
    return current_health > 0;
}

int Monster::status() const {
    return current_health;
}

void Monster::showStatus() const {
    std::cout << "Monster: " << name << "\n";
    std::cout << "Health: " << current_health << " / " << max_health << "\n";
    std::cout << "Attack power: " << power << "\n";
}