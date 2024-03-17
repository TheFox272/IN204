#pragma once

#include <iostream>
#include <cstdlib>
#include <random>
#include <string>

#include <SFML/Graphics.hpp>

#include "entities.hpp"
#include "effects.hpp"

/*----------------------------------------------------------------------------------------------------*/

class Game;

class Player: public sf::Sprite
{
    private:
        int life;
        sf::Texture textures[5];

    public:
        sf::Vector2f inertia;

        Player(bool is_p1): life(5), inertia(0, 0)
        {
            std::string basePath = is_p1 ? "../images/Objects/Player_1/P1_" : "../images/Objects/Player_2/P2_";
            for (size_t i = 0; i < 5; i++) {
                textures[i].loadFromFile(basePath + std::to_string(i + 1) + ".png");
            }
            setTexture(textures[0]);
            setOrigin(getTexture()->getSize().x / 2, getTexture()->getSize().y / 2);
            
            // setRotation(270);
            setScale(0.15, 0.15);
        }

        void update(Game *game);

        void bump(const sf::Sprite obstacle, Game *game, u_int8_t damage);

        const int getLife();

        ~Player()
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
        double progression;
        bool paused;
        bool wasPaused;
        double tileProgress;
        double tileMax;
        Tile *currentTile;
        std::vector<bool> transitionRoads;
        sf::RenderWindow * window;
        sf::Music music;
        sf::SoundBuffer bumpBuffer;
        sf::SoundBuffer bimBuffer;
        sf::SoundBuffer explosionBuffer;

        const bool playerFall(Player &);

    public:
        Player p1;
        Player p2;
        Tile tile1;
        Tile tile2;
        Tile tile3;
        Info score;
        Explosion explosion;
        std::vector<std::unique_ptr<Entity>> entities;
        sf::Sound bumpSound;
        sf::Sound bimSound;
        sf::Sound explosionSound;
    
        Game(sf::RenderWindow * w):
            gen(rd()),
            dis(0.0, 1.0),
            difficulty(1),
            speed(5),
            progression(0),
            paused(false),
            tileProgress(0),
            transitionRoads {false, false, false, false, false},
            window(w),
            p1(true),
            p2(false),
            score(sf::Vector2f(10.f, 10.f))
        {   
            startMusic(music);

            // Load sound effects
            initializeBump(bumpBuffer, bumpSound);
            initializeBim(bimBuffer, bimSound);
            initializeExplosion(explosionBuffer, explosionSound);

            p1.setPosition(sf::Vector2f(window->getSize().x * 0.4, window->getSize().y * 0.7));
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

        int update();

        int updatePause();

        int tileChange();

        int checkCollision();

        const double getSpeed();

        Player *checkDeath();

        int pause();

        int resume();

        int switchPause();

        const bool getPaused();

        Tile *getNextTile(Tile *);

        int gameOver(Player *);

        void addExplosion(const sf::Vector2f &);

        void spawnObstacle(Tile *);

        ~Game()
        {}
};


