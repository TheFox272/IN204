#include "game.hpp"

/*----------------------------------------------------------------------------------------------------*/

#define acceleration 0.001f

#define sideSpeed 5.f
#define thrustSpeed 6.f
#define slowSpeed 5.f
#define pushSpeed 3.f

#define roadWidth 201.f
#define nRoads 5.f

#define changeChance 0.2f

/*----------------------------------------------------------------------------------------------------*/

int Game::update(){

    p1.move(sf::Vector2f(0, -speed));
    p2.move(sf::Vector2f(0, -speed));

    // code for car collision
    if (0){

    }
    // if (p1.getGlobalBounds().intersects(p2.getGlobalBounds())){
        
    // }
    else{
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
    }

    tileProgress += speed;
    if (tileProgress >= tileMax){
        tileProgress -= tileMax;
        tileChange();
    }

    

    const Player *deadPlayer = checkDeath();
    if (deadPlayer != NULL)
        gameOver(deadPlayer);

    speed += acceleration;
    
    score.move(sf::Vector2f(0, -speed));
    score.setString("Speed: " + std::to_string(speed) + " mph");

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

    // std::cout << "beginning :\n";
    // std::cout << "currentTile->roads : ";
    // displayVector(currentTile->roads);
    // std::cout << "oldTileRoads : ";
    // displayVector(oldTileRoads);
    // std::cout << "transitionRoads : ";
    // displayVector(transitionRoads);
    // std::cout << std::endl;

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

    // std::cout << "end :\n";
    // std::cout << "currentTile->roads : ";
    // displayVector(currentTile->roads);
    // std::cout << "oldTileRoads : ";
    // displayVector(oldTileRoads);
    // std::cout << "transitionRoads : ";
    // displayVector(transitionRoads);
    // std::cout << std::endl;

    imagePath += ".png";
    currentTile->texture.loadFromFile(imagePath);
    currentTile->roads.assign(newRoads.begin(), newRoads.end());

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

const Player *Game::checkDeath(){
    if (playerDeath(p1))
        return &p1;
    else if (playerDeath(p2))
        return &p2;
    return NULL;
}

const bool Game::playerDeath(Player &p){
    Tile *tile;
    if (p.getPosition().y >= currentTile->getPosition().y)
        tile = currentTile;
    else if (p.getPosition().y < currentTile->getPosition().y && p.getPosition().y >= getNextTile(currentTile)->getPosition().y)
        tile = getNextTile(currentTile);
    else
        tile = getNextTile(getNextTile(currentTile));
    
    double xMin, xMax;
    xMin = 0;
    xMax = 0;
    u_int8_t i = 0;
    for (auto road = tile->roads.begin(); road != tile->roads.end(); ++road){
        double x = window->getSize().x / 2 - (nRoads / 2 - i) * roadWidth;
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

int Game::gameOver(const Player *deadPlayer){

    pause();
    showExplosion(*window, deadPlayer->getPosition());
    std::cout << "Player " << ((deadPlayer == &p1) ? 1 : 2) << " died..." << std::endl;

    return 0;
}

int Game::pause(){
    if (paused)
        wasPaused = true;
    else
        paused = true;
    return 0;
}

int Game::resume(){
    if (wasPaused)
        wasPaused = false;
    else
        paused = false;
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


