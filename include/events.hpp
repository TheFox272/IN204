#pragma once

#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


void startMusic(sf::Music&);

void stopMusic(sf::Music&);

void playBump();

void showExplosion(sf::RenderWindow&, const sf::Vector2f&);




