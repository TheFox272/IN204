#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "effects.hpp"

/*----------------------------------------------------------------------------------------------------*/

#define tank_speed .5
#define swat_speed .6

/*----------------------------------------------------------------------------------------------------*/


class Entity: public sf::Sprite
{
    protected:
        u_int8_t damage;
        int life;
        sf::Texture texture;
    public:

        Entity(uint8_t damage, int life): damage(damage), life(life)
        {}

        virtual void update() = 0;

        int getLife()
        {
            return life;
        }

        u_int8_t getDamage()
        {
            return damage;
        }

        virtual ~Entity()
        {}

};


class Tank: public Entity
{
    public:
        Tank(double x, double y): Entity(1, 11)
        {
            texture.loadFromFile("../images/Objects/Tank/tank.png");
            setTexture(texture);
            setOrigin(getTexture()->getSize().x / 2, getTexture()->getSize().y / 2);
            setScale(0.4, 0.4);
            setPosition(x, y);
        }

        void update()
        {
            move(0, -tank_speed);
        }

        ~Tank()
        {}

};


class SWAT: public Entity
{
    private:
        sf::Texture textures[5];
    public:
        SWAT(double x, double y): Entity(1, 10)
        {
            for (size_t i = 0; i < 5; i++)
            {
                textures[i].loadFromFile("../images/Objects/SWAT/SWAT_" + std::to_string(i + 1) + ".png");
            }
            setTexture(textures[0]);
            setOrigin(getTexture()->getSize().x / 2, getTexture()->getSize().y / 2);
            setScale(0.3, 0.3);
            setPosition(x, y);
        }

        void update()
        {
            move(0, -swat_speed);
            setTexture(textures[5-(life/2)]);
        }
        
        ~SWAT()
        {}

};

