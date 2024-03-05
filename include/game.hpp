#pragma once

#include <iostream>
#include <cstdlib>
#include <random>
#include <string>

#include <SFML/Graphics.hpp>

#include "events.hpp"

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

            // Mettre l'origine du sprite au centre
            setOrigin(getTexture()->getSize().x / 2, getTexture()->getSize().y / 2);
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

class Tile: public sf::Sprite
{
    public:
        sf::Texture texture;
        std::vector<bool> roads;

        Tile(): roads { false, true, true, true, false }
        {
            texture.loadFromFile("../images/map/road_3.png");
            setTexture(texture);
        }

        double height();

        double width();

        ~Tile()
        {}

};


class Info: public sf::Text
{
    private:
        sf::Font font;

    public:
        Info(const sf::Vector2f &position)
        {
            if (!font.loadFromFile("../fonts/STIXGeneral.ttf"))
                std::cerr << "Erreur lors du chargement de la police" << std::endl;
            setFont(font);
            setCharacterSize(24);
            setFillColor(sf::Color::Black);
            setStyle(Bold);
            setPosition(position);
        }

        ~Info()
        {}
};


class Game
{
    private:
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_real_distribution<double> dis;

        u_int8_t difficulty;  // Difficulty : 1 = easy, 2 = average, 3 = hard
        double speed;
        bool paused;
        bool wasPaused;
        double tileProgress;
        double tileMax;
        Tile *currentTile;
        std::vector<bool> transitionRoads;
        sf::RenderWindow * window;

        const bool playerDeath(Player &);

    public:
        Player p1;
        Player p2;
        Tile tile1;
        Tile tile2;
        Tile tile3;
        Info score;
    
        Game(sf::RenderWindow * w):
            gen(rd()),
            dis(0.0, 1.0),
            difficulty(1),
            speed(5),
            paused(false),
            tileProgress(0),
            transitionRoads {false, false, false, false, false},
            window(w),
            p1(true),
            p2(false),
            score(sf::Vector2f(10.f, 10.f))
        {
            p1.setRotation(270);
            p1.setScale(0.2, 0.2);
            p1.setPosition(sf::Vector2f(window->getSize().x * 0.4, window->getSize().y * 0.7));
            
            p2.setRotation(270);
            p2.setScale(0.2, 0.2);
            p2.setPosition(sf::Vector2f(window->getSize().x * 0.6, window->getSize().y * 0.7));

            double tileScale = window->getSize().x / tile1.width();

            tile1.setScale(tileScale, tileScale);
            tile2.setScale(tileScale, tileScale);
            tile3.setScale(tileScale, tileScale);

            tileMax = tile1.height();
            currentTile = &tile1;

            tile1.setPosition(sf::Vector2f(0, window->getSize().y - tileMax));
            tile2.setPosition(sf::Vector2f(0, window->getSize().y - 2 * tileMax));
            tile3.setPosition(sf::Vector2f(0, window->getSize().y - 3 * tileMax));
        }

        const double getSpeed();

        int update();

        int tileChange();

        int checkCollision();

        const Player *checkDeath();

        int pause();

        int resume();

        int switchPause();

        const bool getPaused();

        Tile *getNextTile(Tile *);

        int gameOver(const Player *);

        ~Game()
        {}
};


