#include "game.hpp"

/*----------------------------------------------------------------------------------------------------*/

#define acceleration 0.001

#define sideSpeed 5.f
#define thrustSpeed 6.f
#define slowSpeed 5.f

/*----------------------------------------------------------------------------------------------------*/

int Game::update(){
    
    p1.move(sf::Vector2f(0.f, -speed));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        p1.move(sf::Vector2f(-sideSpeed, 0.f));
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        p1.move(sf::Vector2f(sideSpeed, 0.f));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        p1.move(sf::Vector2f(0.f, -thrustSpeed));
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        p1.move(sf::Vector2f(0.f, slowSpeed));

    p2.move(sf::Vector2f(0.f, -speed));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        p2.move(sf::Vector2f(-sideSpeed, 0.f));
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        p2.move(sf::Vector2f(sideSpeed, 0.f));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        p2.move(sf::Vector2f(0.f, -thrustSpeed));
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        p2.move(sf::Vector2f(0.f, slowSpeed));

    speed += acceleration;

    return 0;
}

int Game::pause(){
    if (paused)
        return -1;
    else
        paused = true;
    return 0;
}

int Game::resume(){
    if (!paused)
        return -1;
    else
        paused = false;
    return 0;
}

double Game::getSpeed(){
    return speed;
}