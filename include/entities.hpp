#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Config.hpp>
#include <SFML/Main.hpp>
#include <SFML/OpenGL.hpp>

#include "effects.hpp"

/*----------------------------------------------------------------------------------------------------*/

#define sideSpeed 1.f
#define thrustSpeed 6.f
#define slowSpeed 5.f

#define tank_speed .5
#define swat_speed .6

#define bumpSpeed 35.0

#define inertiaLoss 0.8f
#define moveThreshold 0.1f

/*----------------------------------------------------------------------------------------------------*/

// This enum class is used to identify the type of sound to play
enum class SoundType
{
    NONE,
    BUMP,
    BIM,
    EXPLOSION
};

SoundType maxSound(SoundType a, SoundType b);

// The base class for all entities (player, tank, etc.)
class Entity: public sf::Sprite
{
    protected:
        uint8_t life;
        const uint8_t max_life;
        // damage is the amount of life the entity will cause to lose to another entity
        const uint8_t damage;
        // weight will be used to calculate the inertia
        const double weight;
        double speed;
        sf::Vector2f inertia;
        sf::Texture *textures;
    public:
        // constructor
        Entity(uint8_t life, uint8_t damage, double weight, double speed, const char name[10], double scale, double x, double y);

        // getters
        const uint8_t getLife();
        const uint8_t getDamage();

        // setters
        void loseLife(uint8_t damage);

        // bump into an obstacle
        SoundType bump(const sf::Sprite obstacle, u_int8_t damage);
        // bump into a barrier
        SoundType bumpBarrier(const sf::Vector2f barrier);

        // update the entity (move, etc.)
        void update();

        // special update for each entity
        virtual void specialUpdate(sf::Vector2u window_size, double progression, double game_speed) = 0;

        // destructor
        ~Entity();

};


class Player: public Entity
{
    private:
        bool is_p1;
        sf::Keyboard::Key keys[4];
    public:
        Player(double x, double y, bool is_p1);

        void specialUpdate(sf::Vector2u window_size, double progression, double game_speed);

        ~Player()
        {}
};


class Tank: public Entity
{
    public:
        Tank(double x, double y): Entity(2, 1, 4., tank_speed, "Tank", 0.4, x, y)
        {}

        void specialUpdate(sf::Vector2u window_size, double progression, double game_speed);

        ~Tank()
        {}
};




