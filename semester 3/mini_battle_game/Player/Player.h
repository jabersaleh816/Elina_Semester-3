#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Monster;

class Player {
private:
    int current_health;
    int max_health;
    int power;
    int score;
    std::string name;

public:
    Player();
    Player(std::string name, int power, int max_health);
    ~Player();

    void takeDamage(int damage);
    void attackMonster(Monster& monster);
    void heal(int amount);
    bool isAlive() const;
    int status() const;
    void showStatus() const;
    void addScore(int points);
    int getScore() const;
};

#endif