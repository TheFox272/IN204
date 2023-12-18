#include "game.hpp"

/*----------------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------------*/

void Game::movesLeft(int player){
    if (player == 1)
        p1.direction = -1;
    else
        p2.direction = -1;
        
    std::cout << "player" << player << "moves left\n";
}

void Game::movesRight(int player){
    if (player == 1)
        p1.direction = 1;
    else
        p2.direction = 1;
    
    std::cout << "player" << player << "moves right\n";
}

void Game::stops(int player){
    if (player == 1)
        p1.direction = 0;
    else
        p2.direction = 0;

    std::cout << "player" << player << "moves stops\n";
}

int Game::update(){

    return 0;
}

int Game::pause(){
    if (paused)
        return -1;
    else
        paused = true;
    return 0;
}

int Game::resume(){
    if (!paused)
        return -1;
    else
        paused = false;
    return 0;
}

