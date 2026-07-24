#include "Game.h"
#include <iostream>

using namespace std;

Game::Game()
    : player(), monster(), potion(), round(0), game_over(false) {}

Game::~Game() {}

void Game::run() {
    cout << "==============================" << endl;
    cout << "Welcome to Mini Battle Game!" << endl;
    cout << "==============================" << endl;

    while (!game_over) {
        cout << "\n===== Turn " << (round + 1) << " =====" << endl;

        if (!player.isAlive()) {
            cout << "You lost!" << endl;
            game_over = true;
            break;
        }

        if (!monster.isAlive()) {
            cout << "You won!" << endl;
            player.addScore(10);
            game_over = true;
            break;
        }

        cout << "Choose action:" << endl;
        cout << "1. Attack" << endl;
        cout << "2. Use potion" << endl;
        cout << "3. Show status" << endl;
        cout << "4. Exit game" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            player.attackMonster(monster);
            cout << "You attacked the monster." << endl;

            if (!monster.isAlive()) {
                cout << "The monster is defeated." << endl;
                player.addScore(10);
                game_over = true;
                break;
            }

            monster.attackPlayer(player);
            cout << "The monster attacked you." << endl;
        } else if (choice == 2) {
            if (potion.isUsed()) {
                cout << "The potion was already used." << endl;
            } else {
                potion.use(player);
                cout << "You used the potion." << endl;
            }
        } else if (choice == 3) {
            cout << "\n--- Current Status ---" << endl;
            player.showStatus();
            monster.showStatus();
            potion.showStatus();
        } else if (choice == 4) {
            cout << "You exited the game." << endl;
            game_over = true;
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }

        round++;
    }

    cout << "\nGame over!" << endl;
    cout << "Final score: " << player.getScore() << endl;
}
