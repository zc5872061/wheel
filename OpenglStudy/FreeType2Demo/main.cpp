#include <iostream>
#include "game.h"


using namespace std;

int main()
{
    Game* game = new Game(800,600);
    game->start();
    return 0;
}