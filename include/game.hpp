#pragma once

#include <iostream>
#include <cstdlib>


/*----------------------------------------------------------------------------------------------------*/

class Player
{
    private:

    public:
        bool moved;
        int direction;
        int speed;

        Player(): direction(0)
        {}

        ~Player()
        {}

};


class Game
{
    private:
        // Difficulty : 1 = easy, 2 = average, 3 = hard
        int difficulty;
        bool paused;
        Player p1;
        Player p2;

    public:
        Game(): difficulty(1), paused(false)
        {}

        void movesLeft(int player);
        void movesRight(int player);
        void stops(int player);

        int update();

        int pause();

        int resume();

        ~Game()
        {}
};


