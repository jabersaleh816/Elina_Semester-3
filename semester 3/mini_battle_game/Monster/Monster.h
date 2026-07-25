#ifndef MONSTER_H
#define MONSTER_H

#include <string>

class Player; // Forward declaration of the Player class to avoid circular dependency

class Monster {
private:
    std::string name;
    int current_health;
    int max_health;
    int power;

public:
    Monster();
    Monster(const std::string& name, int power, int max_health);
    ~Monster();

    void takeDamage(int damage);
    void attackPlayer(Player& player)const;
    bool isAlive() const;
    int status() const;
    void showStatus() const;
};

#endif
