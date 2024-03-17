#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

/*----------------------------------------------------------------------------------------------------*/

/**
 * @brief This class represents an explosion.
 * 
 * This class inherits from sf::Sprite.
 * The animation of an explosion is a frame by frame animation of 4 sucessive images.
 * It has a contructor Explosion::Explosion() that starts the explosion when called.
 * The method Explosion::update() switches to the next frame.
 * 
 */
class Explosion: public sf::Sprite
{
    private:
        u_int8_t frame;
        sf::Texture textures[64];
    public:
        bool is_displayed;

        Explosion(): frame(0), is_displayed(false)
        {
            for (size_t i = 0; i < 64; i++)
            {
                textures[i].loadFromFile("../images/FX/Explosion/explosion_" + std::to_string(i) + ".png");
            }
            setTexture(textures[0]);
            setOrigin(getTexture()->getSize().x / 2, getTexture()->getSize().y / 2);
            setScale(3, 3);
        }
        /**
         * @brief Switches the animation to the next frame.
         * 
         * It checks if there is a next frame or if the explosion should disapear.
         */
        bool update();

        ~Explosion()
        {}

};

/**
 * @brief This function starts ambiant music.
 * 
 * This function works in pair with @ref stopMusic(sf::Music&) .
 * It handles file loading failures.
 * 
 * @param music is a pointer to a sf::Music type.
 * @return void
 */
void startMusic(sf::Music&);

/**
 * @brief This function stops ambiant music.
 * 
 * This function works in pair with @ref startMusic(sf::Music&) .
 * 
 * @param music The music you want to stop.
 * @return void
 */
void stopMusic(sf::Music&);

/**
 * @brief Plays a bump sound effect.
 * 
 * It handles file loading failures.
 * Its beahvior is similar to @ref initializeExplosion(sf::SoundBuffer&, sf::Sound&)
 *
 * @param buffer Pointer to the buffer where the sound will be loaded from file.
 * @param sound Pointer on the sound environment wich is a sf::Sound.
 */
void initializeBump(sf::SoundBuffer&, sf::Sound&);

/**
 * @brief Plays a bim sound effect.
 * 
 * It handles file loading failures.
 * Its beahvior is similar to @ref initializeBump(sf::SoundBuffer&, sf::Sound&)
 *
 * @param buffer Pointer to the buffer where the sound will be loaded from file.
 * @param sound Pointer on the sound environment wich is a sf::Sound.
 */
void initializeBim(sf::SoundBuffer&, sf::Sound&);

/**
 * @brief Plays an explosion sound effect.
 * 
 * It handles file loading failures.
 * Its beahvior is similar to @ref initializeBump(sf::SoundBuffer&, sf::Sound&)
 *
 * @param buffer Pointer to the buffer where the sound will be loaded from file.
 * @param sound Pointer on the sound environment wich is a sf::Sound.
 */
void initializeExplosion(sf::SoundBuffer&, sf::Sound&);
