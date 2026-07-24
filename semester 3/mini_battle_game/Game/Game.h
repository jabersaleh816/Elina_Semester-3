#ifndef GAME_H
#define GAME_H

#include "../Player/Player.h"
#include "../Monster/Monster.h"
#include "../Potion/Potion.h"

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