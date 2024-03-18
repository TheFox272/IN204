#include "game.hpp"

/*----------------------------------------------------------------------------------------------------*/

#define acceleration 0.001f

#define sideSpeed 5.f
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

    progression += speed;
    
    p1.update(this);
    p2.update(this);

    if (p1.getGlobalBounds().intersects(p2.getGlobalBounds())){
        p1.bump(p2, this, 0);
        p2.bump(p1, this, 0);
    }

    if (p1.inertia.x > moveThreshold || p1.inertia.x < -moveThreshold){
        p1.move(sf::Vector2f(p1.inertia.x, 0));
        p1.inertia.x = p1.inertia.x * inertiaLoss;
    }
    if (p1.inertia.y > moveThreshold || p1.inertia.y < -moveThreshold){
        p1.move(sf::Vector2f(0, p1.inertia.y));
        p1.inertia.y = p1.inertia.y * inertiaLoss;
    }
    if (p2.inertia.x > moveThreshold || p2.inertia.x < -moveThreshold){
        p2.move(sf::Vector2f(p2.inertia.x, 0));
        p2.inertia.x = p2.inertia.x * inertiaLoss;
    }
    if (p2.inertia.y > moveThreshold || p2.inertia.y < -moveThreshold){
        p2.move(sf::Vector2f(0, p2.inertia.y));
        p2.inertia.y = p2.inertia.y * inertiaLoss;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        p1.move(sf::Vector2f(-sideSpeed, 0));
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        p1.move(sf::Vector2f(sideSpeed, 0));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        p1.move(sf::Vector2f(0, -thrustSpeed));
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        p1.move(sf::Vector2f(0, slowSpeed));
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        p2.move(sf::Vector2f(-sideSpeed, 0));
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        p2.move(sf::Vector2f(sideSpeed, 0));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        p2.move(sf::Vector2f(0, -thrustSpeed));
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        p2.move(sf::Vector2f(0, slowSpeed));

    if (p1.getPosition().y > window->getSize().y - progression - p1.getGlobalBounds().height / 2)
        p1.setPosition(p1.getPosition().x, window->getSize().y - progression - p1.getGlobalBounds().height / 2);
    else if (p1.getPosition().y < -progression + p1.getGlobalBounds().height / 2)
        p1.setPosition(p1.getPosition().x, -progression + p1.getGlobalBounds().height / 2);
    if (p2.getPosition().y > window->getSize().y - progression - p2.getGlobalBounds().height / 2)
        p2.setPosition(p2.getPosition().x, window->getSize().y - progression - p2.getGlobalBounds().height / 2);
    else if (p2.getPosition().y < -progression + p2.getGlobalBounds().height / 2)
        p2.setPosition(p2.getPosition().x, -progression + p2.getGlobalBounds().height / 2);

    for (auto &entity : entities){
        entity->update();
        if (entity->getGlobalBounds().intersects(p1.getGlobalBounds())){
            p1.bump(*entity, this, entity->getDamage());
        }
        if (entity->getGlobalBounds().intersects(p2.getGlobalBounds())){
            p2.bump(*entity, this, entity->getDamage());
        }
    }

    tileProgress += speed;
    if (tileProgress >= tileMax){
        tileProgress -= tileMax;
        tileChange();
    }

    Player *deadPlayer = checkDeath();
    if (deadPlayer != NULL)
        gameOver(deadPlayer);

    speed += acceleration;
    
    score.move(sf::Vector2f(0, -speed));
    score.setString("Speed: " + std::to_string(speed).substr(0, 5) + " mph");

    return 0;
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

void displayVector(const std::vector<bool>& vec) {
    for (bool value : vec) {
        std::cout << std::boolalpha << value << " "; // std::boolalpha affiche true/false au lieu de 1/0
    }
    std::cout << std::endl;
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
    else if (p2.getLife() <= 0 || playerFall(p2))
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


void Player::update(Game *game){
    move(sf::Vector2f(0, -game->getSpeed()));
    setTexture(textures[5-life]);
}


void Player::bump(const sf::Sprite obstacle, Game *game, u_int8_t damage){
    if (damage > 0){
        game->bimSound.play();
    } else {
        game->bumpSound.play();
    }
    game->bumpSound.play();
    inertia.x += (getPosition().x - obstacle.getPosition().x) * XpushSpeed / (getGlobalBounds().width + obstacle.getGlobalBounds().width);
    inertia.y += (getPosition().y - obstacle.getPosition().y) * YpushSpeed / (getGlobalBounds().height + obstacle.getGlobalBounds().height);
    life -= damage;
}

const int Player::getLife(){
    return life;
}
