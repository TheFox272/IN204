#include "entities.hpp"

/*----------------------------------------------------------------------------------------------------*/

SoundType maxSound(SoundType a, SoundType b)
{
    if (a == SoundType::EXPLOSION || b == SoundType::EXPLOSION)
    {
        return SoundType::EXPLOSION;
    }
    else if (a == SoundType::BIM || b == SoundType::BIM)
    {
        return SoundType::BIM;
    }
    else if (a == SoundType::BUMP || b == SoundType::BUMP)
    {
        return SoundType::BUMP;
    }
    return SoundType::NONE;
}

/*----------------------------------------------------------------------------------------------------*/

Entity::Entity(uint8_t life, uint8_t damage, double weight, double speed, const char name[10], double scale, double x, double y):
    life(life),
    max_life(life),
    damage(damage),
    weight(weight),
    speed(speed),
    inertia(0, 0)
{  
    textures = new sf::Texture[max_life];
    // load images from ../images/Objects/name/name_i.png where i is the image corresponding to the object when it life equals max_life - i
    for (size_t i = 0; i < max_life; i++)
    {
        textures[i].loadFromFile("../images/Objects/" + std::string(name) + "/" + std::string(name) + "_" + std::to_string(i+1) + ".png");
    }
    setTexture(textures[0]);
    setOrigin(getTexture()->getSize().x / 2, getTexture()->getSize().y / 2);
    setScale(scale, scale);
    setPosition(x, y);
}

Entity::~Entity()
{
    delete[] textures;
}

const uint8_t Entity::getLife()
{
    return life;
}

const uint8_t Entity::getDamage()
{
    return damage;
}

void Entity::loseLife(uint8_t damage)
{
    life -= damage;
    if (life < 0)
    {
        life = 0;
    }
    setTexture(textures[max_life - life]);
}

SoundType Entity::bump(const sf::Sprite obstacle, u_int8_t damage)
{   
    inertia.x += (getPosition().x - obstacle.getPosition().x) * (bumpSpeed/weight) / (getGlobalBounds().width + obstacle.getGlobalBounds().width);
    inertia.y += (getPosition().y - obstacle.getPosition().y) * (bumpSpeed/weight) / (getGlobalBounds().height + obstacle.getGlobalBounds().height);

    loseLife(damage);
    if (life == 0)
    {
        return SoundType::EXPLOSION;
    }
    else if (damage == 0)
    {
        return SoundType::BUMP;
    }
    return SoundType::BIM;
}

void Entity::update()
{   
    // basic movement
    move(sf::Vector2f(0, -speed));

    // inertia
    if (inertia.x > moveThreshold || inertia.x < -moveThreshold){
        move(sf::Vector2f(inertia.x, 0));
        inertia.x = inertia.x * inertiaLoss;
    }
    if (inertia.y > moveThreshold || inertia.y < -moveThreshold){
        move(sf::Vector2f(0, inertia.y));
        inertia.y = inertia.y * inertiaLoss;
    }
}

/*----------------------------------------------------------------------------------------------------*/

Player::Player(double x, double y, bool is_p1):
    Entity(5, 0, 1.0, 0.0, is_p1 ? "P1" : "P2", 0.15, x, y),
    is_p1(is_p1)
{
    keys[0] = is_p1 ? sf::Keyboard::Z : sf::Keyboard::Up;
    keys[1] = is_p1 ? sf::Keyboard::S : sf::Keyboard::Down;
    keys[2] = is_p1 ? sf::Keyboard::Q : sf::Keyboard::Left;
    keys[3] = is_p1 ? sf::Keyboard::D : sf::Keyboard::Right;
}

void Player::specialUpdate(sf::Vector2u window_size, double progression, double game_speed)
{
    speed = game_speed;
    
    if (sf::Keyboard::isKeyPressed(keys[2]) && !sf::Keyboard::isKeyPressed(keys[3]))
        move(sf::Vector2f(-sideSpeed * speed, 0));
    else if (!sf::Keyboard::isKeyPressed(keys[2]) && sf::Keyboard::isKeyPressed(keys[3]))
        move(sf::Vector2f(sideSpeed * speed, 0));
    if (sf::Keyboard::isKeyPressed(keys[0]) && !sf::Keyboard::isKeyPressed(keys[1]))
        move(sf::Vector2f(0, -thrustSpeed));
    else if (!sf::Keyboard::isKeyPressed(keys[0]) && sf::Keyboard::isKeyPressed(keys[1]))
        move(sf::Vector2f(0, slowSpeed));
    
    if (getPosition().y > window_size.y - progression - getGlobalBounds().height / 2)
        setPosition(getPosition().x, window_size.y - progression - getGlobalBounds().height / 2);
    else if (getPosition().y < -progression + getGlobalBounds().height / 2)
        setPosition(getPosition().x, -progression + getGlobalBounds().height / 2);
}


/*----------------------------------------------------------------------------------------------------*/

void Tank::specialUpdate(sf::Vector2u window_size, double progression, double game_speed)
{
    // if (getPosition().y > window_size.y - progression + getGlobalBounds().height / 2)
}


