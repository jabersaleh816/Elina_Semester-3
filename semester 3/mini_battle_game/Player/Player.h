#ifndef PLAYER_H // Header guard to prevent multiple inclusions
#define PLAYER_H 

#include <string>

class Monster;

class Player { 
private: // Private member variables
    int current_health;
    int max_health;
    int power;
    int score;
    std::string name;

public:
    Player(); // Default constructor
    Player(const std::string& name, int power, int max_health); // Parameterized constructor and overloaded constructor
    ~Player();

    void takeDamage(int damage);
    void attackMonster(Monster& monster) const;
    void heal(int amount);
    bool isAlive() const;
    int status() const;
    void showStatus() const;
    void addScore(int points);
    int getScore() const;
};

#endif