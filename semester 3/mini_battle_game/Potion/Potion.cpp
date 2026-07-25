#include "../Player/Player.h" // Include the Player class header file
#include "Potion.h" // Include the Potion class header file
#include <iostream>

Potion::Potion() : Potion("Default_Potion", 20) {}// Default constructor with default values

Potion::Potion(const std::string& name, int healing_amount) // Parameterized constructor and overloaded constructor
    : name(name),
      healing_amount(healing_amount),
      used(false) {
    if (healing_amount <= 0) {
        this->healing_amount = 20;
    }
}

Potion::~Potion() {} // Destructor

void Potion::use(Player& player) {
    if (!used) {
        player.heal(healing_amount);
        used = true;
    }
}

bool Potion::isUsed() const {
    return used;
}

int Potion::status() const {
    return healing_amount;
}

void Potion::showStatus() const {
    std::cout << "Potion: " << name << "\n";
    std::cout << "Healing amount: " << healing_amount << "\n";
    std::cout << "Used: " << (used ? "Yes" : "No") << "\n";
}