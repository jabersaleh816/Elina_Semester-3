#ifndef GAME_H
#define GAME_H

#include "Monster.h"
#include "Potion.h"
#include "Player.h"


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