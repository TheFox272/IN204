#include "game.hpp"

/*----------------------------------------------------------------------------------------------------*/

#define acceleration 0.001f

#define sideSpeed 1.f
#define thrustSpeed 6.f
#define slowSpeed 5.f
#define XpushSpeed 30.f
#define YpushSpeed 35.f
#define inertiaLoss 0.8f
#define moveThreshold 0.1f

#define roadWidth 201.f
#define nRoads 5.f

#define changeChance 0.3f
#define spawnChance 0.5f

#define spawnMargin 0.4f

/*----------------------------------------------------------------------------------------------------*/

int Game::updatePause(){
    if (explosion.is_displayed){
        sf::sleep(sf::milliseconds(10));
        if (!explosion.update()){
            explosion.is_displayed = false;
        }
    }
    return 0;
}

int Game::update(){

    speed += acceleration;
    progression += speed;
    
    SoundType sound = SoundType::NONE;

    // players update
    p1.update();
    p1.specialUpdate(window->getSize(), progression, speed);
    if (!solo){
        p2.update();
        p2.specialUpdate(window->getSize(), progression, speed);
        if (p1.getGlobalBounds().intersects(p2.getGlobalBounds())){
            sound = maxSound(sound, p1.bump(p2, p2.getDamage()));
            sound = maxSound(sound, p2.bump(p1, p1.getDamage()));
        }
    }

    // entities update
    for (auto &entity : entities){
        entity->update();
        entity->specialUpdate(window->getSize(), progression, speed);
        if (entity->getGlobalBounds().intersects(p1.getGlobalBounds())){
            sound = maxSound(sound, p1.bump(*entity, entity->getDamage()));
            sound = maxSound(sound, entity->bump(p1, p1.getDamage()));
        }
        if (!solo){
            if (entity->getGlobalBounds().intersects(p2.getGlobalBounds())){
                sound = maxSound(sound, p2.bump(*entity, entity->getDamage()));
                sound = maxSound(sound, entity->bump(p2, p2.getDamage()));
            }
        }
    }

    if (sound == SoundType::BUMP)
        bumpSound.play();
    else if (sound == SoundType::BIM)
        bimSound.play();
    else if (sound == SoundType::EXPLOSION)
        explosionSound.play();

    // tiles update
    tileProgress += speed;
    if (tileProgress >= tileMax){
        tileProgress -= tileMax;
        tileChange();
    }

    Player *deadPlayer = checkDeath();
    if (deadPlayer != NULL)
        gameOver(deadPlayer);

    score.move(sf::Vector2f(0, -speed));
    score.setString("Speed: " + std::to_string(speed).substr(0, 5) + " mph");

    return 0;
}


void Game::spawnObstacle(Tile *tile){
    double r = dis(gen);
    if (r < spawnChance){
        double xMin = 0;
        double xMax = 0;
        double yMin = 0;
        double yMax = 0;
        u_int8_t i = 0;
        for (auto road = tile->roads.begin(); road != tile->roads.end(); ++road){
            double x = window->getSize().x / 2 - (nRoads / 2. - i) * roadWidth;
            if (*road && xMin == 0)
                xMin = x;
            else if (!(*road) && xMin != 0){
                xMax = x;
                break;
            }
            i++;
        }
        xMin = xMin + spawnMargin * roadWidth;
        xMax = xMax - spawnMargin * roadWidth;
        yMin = tile->getPosition().y + spawnMargin * tileMax;
        yMax = tile->getPosition().y + tileMax - spawnMargin * tileMax;
        double x = xMin + (xMax - xMin) * dis(gen);
        double y = yMin + (yMax - yMin) * dis(gen);
        entities.push_back(std::make_unique<Tank>(x, y));
    }
}


bool areEqual(const std::vector<bool>& roads1, const std::vector<bool>& roads2) {
    if (roads1.size() != roads2.size()) {
        return false;
    }
    for (size_t i = 0; i < roads1.size(); ++i) {
        if (roads1[i] != roads2[i]) {
            return false;
        }
    }
    return true;
}

int Game::tileChange(){

    (*currentTile).move(sf::Vector2f(0, -3 * tileMax));

    std::string imagePath = "../images/map/road_";
    std::vector<bool> newRoads;
    std::vector<bool> oldTileRoads;
    double r;
    
    if (areEqual(transitionRoads, std::vector<bool>{ false, false, false, false, false})){
        Tile *oldTile = getNextTile(getNextTile(currentTile));
        oldTileRoads.assign(oldTile->roads.begin(), oldTile->roads.end());
        r = dis(gen);
    }
    else{
        oldTileRoads.assign(transitionRoads.begin(), transitionRoads.end());
        r = 1;
    }

    if (r < changeChance){
        if (areEqual(oldTileRoads, std::vector<bool>{ false, true, true, true, false })){
            imagePath += "3_to_";
            if (r < changeChance / 2){
                imagePath += "2_right";
                newRoads = { false, false, true, true, false };
                transitionRoads.assign(newRoads.begin(), newRoads.end());
            }
            else{
                imagePath += "2_left";
                newRoads = { false, true, true, false, false };
                transitionRoads.assign(newRoads.begin(), newRoads.end());
            }
        }
        else if (areEqual(oldTileRoads, std::vector<bool>{ false, false, true, true, false })){
            imagePath += "2_right_to_3";
            newRoads = { false, false, true, true, false };
            transitionRoads = { false, true, true, true, false };
        }
        else if (areEqual(oldTileRoads, std::vector<bool>{ false, true, true, false, false })){
            imagePath += "2_left_to_3";
            newRoads = { false, true, true, false, false };
            transitionRoads = { false, true, true, true, false };
        }
        else{
            //TODO
        }
    }
    else{
        if (areEqual(oldTileRoads, std::vector<bool>{ false, true, true, true, false })){
            imagePath += "3";
            newRoads = { false, true, true, true, false };
        }
        else if (areEqual(oldTileRoads, std::vector<bool>{ false, false, true, true, false })){
            imagePath += "2_right";
            newRoads = { false, false, true, true, false };
        }
        else if (areEqual(oldTileRoads, std::vector<bool>{ false, true, true, false, false })){
            imagePath += "2_left";
            newRoads = { false, true, true, false, false };
        }
        else{
            //TODO
        }
        transitionRoads = { false, false, false, false, false};
    }

    imagePath += ".png";
    currentTile->texture.loadFromFile(imagePath);
    currentTile->roads.assign(newRoads.begin(), newRoads.end());
    spawnObstacle(currentTile);

    currentTile = getNextTile(currentTile);

    return 0;
}

Tile *Game::getNextTile(Tile *tile){
    if (tile == &tile1)
        return &tile2;
    else if (tile == &tile2)
        return &tile3;
    else
        return &tile1;
}

Player *Game::checkDeath(){
    if (p1.getLife() <= 0 || playerFall(p1))
        return &p1;
    else if (!solo && (p2.getLife() <= 0 || playerFall(p2)))
        return &p2;
    return NULL;
}

const bool Game::playerFall(Player &p){
    Tile *tile;
    if (p.getPosition().y >= currentTile->getPosition().y)
        tile = currentTile;
    else if (p.getPosition().y < currentTile->getPosition().y && p.getPosition().y >= getNextTile(currentTile)->getPosition().y)
        tile = getNextTile(currentTile);
    else
        tile = getNextTile(getNextTile(currentTile));
    
    double xMin = 0;
    double xMax = 0;
    u_int8_t i = 0;
    for (auto road = tile->roads.begin(); road != tile->roads.end(); ++road){
        double x = window->getSize().x / 2 - (nRoads / 2. - i) * roadWidth;
        if (*road && xMin == 0)
            xMin = x;
        else if (!(*road) && xMin != 0){
            xMax = x;
            break;
        }
        i++;
    }
    if (p.getPosition().x < xMin || p.getPosition().x > xMax){
        return true;
    }
    return false;
}

int Game::gameOver(Player *deadPlayer){

    pause();
    addExplosion(deadPlayer->getPosition());
    deadPlayer->setColor(sf::Color(255, 255, 255, 0));
    std::cout << "Player " << ((deadPlayer == &p1) ? 1 : 2) << " died..." << std::endl;

    return 0;
}

void Game::addExplosion(const sf::Vector2f& position){
    explosion.setPosition(position);
    explosion.is_displayed = true;
    explosionSound.play();
};

int Game::pause(){
    if (paused)
        wasPaused = true;
    else{
        paused = true;
        music.pause();
    }
    return 0;
}

int Game::resume(){
    if (wasPaused)
        wasPaused = false;
    else{
        paused = false;
        music.play();
    }
    return 0;
}

int Game::switchPause(){
    if (paused)
        return resume();
    else
        return pause();
}

const double Game::getSpeed(){
    return speed;
}

const bool Game::getPaused(){
    return paused;
}

double Tile::height(){
    return texture.getSize().y * getScale().y;
}

double Tile::width(){
    return texture.getSize().x * getScale().x;
}


// void Player::update(Game *game){
//     // update position
//     move(sf::Vector2f(0, -game->getSpeed()));
//     if (inertia.x > moveThreshold || inertia.x < -moveThreshold){
//         move(sf::Vector2f(inertia.x, 0));
//         inertia.x = inertia.x * inertiaLoss;
//     }
//     if (inertia.y > moveThreshold || inertia.y < -moveThreshold){
//         move(sf::Vector2f(0, inertia.y));
//         inertia.y = inertia.y * inertiaLoss;
//     }

//     // update texture
//     setTexture(textures[5-life]);
// }


// void Player::bump(const sf::Sprite obstacle, Game *game, u_int8_t damage){
//     if (damage > 0){
//         game->bimSound.play();
//     } else {
//         game->bumpSound.play();
//     }
//     game->bumpSound.play();
//     inertia.x += (getPosition().x - obstacle.getPosition().x) * XpushSpeed / (getGlobalBounds().width + obstacle.getGlobalBounds().width);
//     inertia.y += (getPosition().y - obstacle.getPosition().y) * YpushSpeed / (getGlobalBounds().height + obstacle.getGlobalBounds().height);
//     life -= damage;
// }

// const int Player::getLife(){
//     return life;
// }
