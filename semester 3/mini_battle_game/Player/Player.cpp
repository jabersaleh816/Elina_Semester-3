#include "Player.h"
#include "../Monster/Monster.h"
#include <iostream>

Player::Player() : Player("Default_Player", 20, 100) {}

Player::Player(const std::string& name, int power, int max_health)
    : current_health(max_health),
      max_health(max_health),
      power(power),
      score(0),
      name(name.empty() ? "Default_Player" : name) {
    if (max_health <= 0) {
        this->max_health = 100;
    }
    if (power <= 0) {
        this->power = 20;
    }
    current_health = this->max_health;
}

Player::~Player() {}

void Player::takeDamage(int damage) {
    if (damage < 0) {
        damage = 0;
    }
    current_health -= damage;
    if (current_health < 0) {
        current_health = 0;
    }
}

void Player::attackMonster(Monster& monster) const{
    monster.takeDamage(power);
}

void Player::heal(int amount) {
    if (amount > 0) {
        current_health += amount;
        if (current_health > max_health) {
            current_health = max_health;
        }
    }
}

bool Player::isAlive() const {
    return current_health > 0;
}

int Player::status() const {
    return current_health;
}

void Player::showStatus() const {
    std::cout << "Player: " << name << "\n";
    std::cout << "Health: " << current_health << " / " << max_health << "\n";
    std::cout << "Attack power: " << power << "\n";
    std::cout << "Score: " << score << "\n";
}

void Player::addScore(int points) {
    if (points > 0) {
        score += points;
    }
}

int Player::getScore() const {
    return score;
}

