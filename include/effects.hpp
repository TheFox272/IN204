#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

/*----------------------------------------------------------------------------------------------------*/

class Explosion: public sf::Sprite
{
    private:
        u_int8_t frame;
        sf::Texture textures[4];
    public:
        bool is_displayed;

        Explosion(): frame(0), is_displayed(false)
        {
            for (size_t i = 0; i < 4; i++)
            {
                textures[i].loadFromFile("../animations/explosion/explosion_frame" + std::to_string(i) + ".png");
            }
            setTexture(textures[0]);
            setOrigin(getTexture()->getSize().x / 2, getTexture()->getSize().y / 2);
            setScale(0.2, 0.2);
        }

        bool update();

        ~Explosion()
        {}

};

void startMusic(sf::Music&);

void stopMusic(sf::Music&);

void initializeBump(sf::SoundBuffer&, sf::Sound&);

void initializeExplosion(sf::SoundBuffer&, sf::Sound&);
