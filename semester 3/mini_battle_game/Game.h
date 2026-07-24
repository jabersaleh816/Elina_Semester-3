#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Monster.h"
#include "Potion.h"

class Game {
private:
    Player player;
    Monster monster;
    Potion potion;
    int round;
    bool game_over;

public:
    Game();
    ~Game();
    void run();
};

#endif