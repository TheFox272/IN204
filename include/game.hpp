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

class Tile: public sf::Sprite
{
    public:
        sf::Texture texture;
        std::vector<bool> roads;

        // Constructor
        Tile();

        // Getters
        double height();
        double width();

        // Setters
        ~Tile()
        {}
};


class Info: public sf::Text
{
    private:
        sf::Font font;

    public:
        Info(const sf::Vector2f &position);

        ~Info()
        {}
};


class HpBar: public sf::Sprite
{
    private:
        bool isPlayer1;

    public:
        sf::Texture textures[6];
        HpBar(const sf::Vector2f &position, bool isPlayer1);

        ~HpBar()
        {}
};


class Game
{
    private:
        // Random number generator and distribution
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_real_distribution<double> dis;
        // Whether the game is solo or not
        bool solo;
        // Difficulty : 1 = easy, 2 = average, 3 = hard
        u_int8_t difficulty;
        // Speed of the game
        double speed;
        // Progression of the game
        double progression;
        // Whether the game is paused or not
        bool paused;
        bool wasPaused;
        // Progression of the tile
        double tileProgress;
        double tileMax;
        Tile *currentTile;
        std::vector<bool> transitionRoads;
        // Window
        sf::RenderWindow * window;
        // Music and sound effects
        sf::Music music;
        sf::SoundBuffer bumpBuffer;
        sf::SoundBuffer bimBuffer;
        sf::SoundBuffer explosionBuffer;
        // Check weither a player is out of the road or not
        const bool playerFall(Player &);
    public:
        Player p1;
        Player p2;
        HpBar hpBar1;
        HpBar hpBar2;
        Tile tile1;
        Tile tile2;
        Tile tile3;
        Info score;
        Explosion explosion;
        std::vector<std::unique_ptr<Entity>> entities;
        sf::Sound bumpSound;
        sf::Sound bimSound;
        sf::Sound explosionSound;
    
        Game(sf::RenderWindow * w, bool solo);

        // Main game loop
        int update();

        // Pause management
        int updatePause();
        int pause();
        int resume();
        int switchPause();
        const bool getPaused();

        // Tile management
        int tileChange();
        Tile *getNextTile(Tile *);
        
        // Player management
        int checkCollision();
        Player *checkDeath();

        const double getSpeed();

        void addExplosion(const sf::Vector2f &);

        void spawnObstacle(Tile *);

        int gameOver(Player *);

        ~Game()
        {}
};


