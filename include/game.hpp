#pragma once

#include <iostream>
#include <cstdlib>

#include <SFML/Graphics.hpp>

/*----------------------------------------------------------------------------------------------------*/

class Player: public sf::Sprite
{
    private:
        int life;
        sf::Texture texture;
    public:

        Player(bool is_p1): life(3)
        {
            if (is_p1)
                texture.loadFromFile("../images/player1.png");
            else
                texture.loadFromFile("../images/player2.png");
            setTexture(texture);
        }

        ~Player()
        {}

};


class Pothole: public sf::Sprite
{
    private:
        sf::Texture texture;
    public:

        Pothole()
        {
            texture.loadFromFile("../images/pothole.png");
            setTexture(texture);
        }

        ~Pothole()
        {}

};


class Game
{
    private:
        // Difficulty : 1 = easy, 2 = average, 3 = hard
        int difficulty;
        double speed;
        bool paused;

    public:
        Player p1;
        Player p2;

        Game(sf::Vector2u wSize): difficulty(1), speed(5), paused(false), p1(true), p2(false)
        {
            p1.setPosition(sf::Vector2f(wSize.x * 0.4, wSize.y * 0.7));
            p1.setRotation(270);
            p1.setScale(0.2, 0.2);

            p2.setPosition(sf::Vector2f(wSize.x * 0.6, wSize.y * 0.7));
            p2.setRotation(270);
            p2.setScale(0.2, 0.2);
        }

        double getSpeed();

        int update();

        int pause();

        int resume();

        ~Game()
        {}
};


