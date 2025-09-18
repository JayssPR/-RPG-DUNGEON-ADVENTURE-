#include "game.h"
#include <cstdlib>
#include <ctime>
using namespace std;

// Main entry point of the game
int main() {
    srand(time(0));  // Seed the random number generator with the current time
    Game game;       // Create a Game object
    game.start();    // Start the main game loop
    return 0;        // Exit the program
}
