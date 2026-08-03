#ifndef POTION_H
#define POTION_H

#include <string>

class Player;

class Potion {
private:
    std::string name;
    int healing_amount;
    bool used;

public:
    Potion();
    Potion(const std::string& name, int healing_amount);
    ~Potion();

    void use(Player& player);
    bool isUsed() const;
    int status() const;
    void showStatus() const;
};

#endif