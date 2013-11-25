/* main.cpp
 * 
 * Entry point for the game.
 *
 */

#include "game.h"

int main(int argc, char* argv[]) {
    Game theGame(&argc, argv);
    theGame.run();
    return 0;
}
