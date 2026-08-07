#include "Game.h" // Include the Game class header file
#include <iostream>

using namespace std;



Game::Game() // Constructor
    : player(), monster(), potion(), round(0), game_over(false) {}

Game::~Game() {} // Destructor

void Game::run() {  // Main game loop
    cout << "==============================" << endl;
    cout << "Welcome to Mini Battle Game!" << endl;
    cout << "==============================" << endl;

    while (!game_over) { // Main game loop
        cout << "\n===== Turn " << (round + 1) << " =====" << endl;

        if (!player.isAlive()) { // Check if the player is alive
            cout << "You lost!" << endl;
            game_over = true;
            break;
        }

        if (!monster.isAlive()) { // Check if the monster is alive
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

        int choice = 0;
        cin >> choice; 

        if (cin.fail()) { // Check for invalid input
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid choice. Please try again." << endl;
            continue;
        }

        switch (choice) { // Handle player's choice
            case 1:
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
                break;
            case 2:
                if (potion.isUsed()) {
                    cout << "The potion was already used." << endl;
                } else {
                    potion.use(player);
                    cout << "You used the potion." << endl;
                }
                break;
            case 3:
                cout << "\n--- Current Status ---" << endl;
                player.showStatus();
                monster.showStatus();
                potion.showStatus();
                break;
            case 4:
                cout << "You exited the game." << endl;
                game_over = true;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }

        round++;
    }

    cout << "\nGame over!" << endl;
    cout << "Final score: " << player.getScore() << endl;
}
