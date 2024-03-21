#include "game.hpp"

/*----------------------------------------------------------------------------------------------------*/

#define roadWidth 185.f
#define nRoads 5.f

#define spawnMargin 0.4f

#define barrierPush 50.f

/*----------------------------------------------------------------------------------------------------*/

Tile::Tile():
    roads { false, true, true, true, false }
{
    roadNbTable["road_3"] = 0;
    roadNbTable["road_3_to_2_right"] = 1;
    roadNbTable["road_3_to_2_left"] = 2;
    roadNbTable["road_2_right_to_3"] = 3;
    roadNbTable["road_2_left_to_3"] = 4;
    roadNbTable["road_2_right"] = 5;
    roadNbTable["road_2_left"] = 6;

    textures[0].loadFromFile("../images/map/road_3.png");
    textures[1].loadFromFile("../images/map/road_3_to_2_right.png");
    textures[2].loadFromFile("../images/map/road_3_to_2_left.png");
    textures[3].loadFromFile("../images/map/road_2_right_to_3.png");
    textures[4].loadFromFile("../images/map/road_2_left_to_3.png");
    textures[5].loadFromFile("../images/map/road_2_right.png");
    textures[6].loadFromFile("../images/map/road_2_left.png");

    roadNb = 0;
    setTexture(textures[roadNbTable["road_3"]]);
}

const uint8_t Tile::getRoadNb(){
    return roadNb;
}

const double Tile::getHeight(){
    return textures[0].getSize().y * getScale().y;
}

const double Tile::getWidth(){
    return textures[0].getSize().x * getScale().x;
}

void Tile::setRoads(std::vector<bool> newRoads){
    roads.assign(newRoads.begin(), newRoads.end());
}

void Tile::setRoadNb(std::string roadString){
    if (roadNbTable[roadString] != roadNb){
        roadNb = roadNbTable[roadString];
        setTexture(textures[roadNb]);
    }
}

/*----------------------------------------------------------------------------------------------------*/

Info::Info(const sf::Vector2f &position)
{
    if (!font.loadFromFile("../fonts/STIXGeneral.ttf"))
        std::cerr << "Erreur lors du chargement de la police" << std::endl;
    setFont(font);
    setCharacterSize(30);
    setFillColor(sf::Color::White);
    setStyle(Bold);
    setPosition(position);
}

/*----------------------------------------------------------------------------------------------------*/

HpBar::HpBar(const sf::Vector2f &position, bool isPlayer1):
    isPlayer1(isPlayer1)
{
    for (size_t i = 0; i < 6; i++)
    {
        textures[i].loadFromFile("../images/FX/HP/P" + std::to_string(isPlayer1 ? 1 : 2) + "_" + std::to_string(i) + ".png");
    }
    setTexture(textures[4]);
    setOrigin(getTexture()->getSize().x / 2, getTexture()->getSize().y / 2);
    setScale(0.6, 0.6);
    setPosition(position);
}

/*----------------------------------------------------------------------------------------------------*/

Game::Game(sf::RenderWindow * w, bool solo, uint8_t difficulty):
    gen(rd()),
    dis(0.0, 1.0),
    solo(solo),
    difficulty(difficulty),
    progression(0),
    paused(false),
    tileProgress(0),
    transitionRoads {false, false, false, false, false},
    window(w),
    gameover(false),
    p1(window->getSize().x * (solo ? 0.5 : 0.4), window->getSize().y * 0.7, true),
    p2(window->getSize().x * (solo ? 0.8 : 0.6), window->getSize().y * (solo ? 0.3 : 0.7), false),
    hpBar1(sf::Vector2f(200, 150), true),
    hpBar2(sf::Vector2f(200, 270), false),
    score(sf::Vector2f(10.f, 10.f))
{   
    // Difficulty setup
    if (difficulty == 1){
        speed = 5;
        acceleration = 0.001f;
        spawnChance = 0.3f;
        changeChance = 0.3f;
    }
    else if (difficulty == 2){
        speed = 7;
        acceleration = 0.002f;
        spawnChance = 0.5f;
        changeChance = 0.5f;
    }
    else{
        speed = 10;
        acceleration = 0.004f;
        spawnChance = 0.7f;
        changeChance = 0.7f;
    }

    startMusic(music);

    // Load sound effects
    initializeBump(bumpBuffer, bumpSound);
    initializeBim(bimBuffer, bimSound);
    initializeExplosion(explosionBuffer, explosionSound);

    if (solo){
        p2.setRotation(50);
        p2.loseLife(4);
    }

    double tileScale = window->getSize().x / tile1.getWidth();

    tile1.setScale(tileScale, tileScale);
    tile2.setScale(tileScale, tileScale);
    tile3.setScale(tileScale, tileScale);

    tileMax = tile1.getHeight();
    currentTile = &tile1;

    tile1.setPosition(sf::Vector2f(0, window->getSize().y - tileMax));
    tile2.setPosition(sf::Vector2f(0, window->getSize().y - 2 * tileMax));
    tile3.setPosition(sf::Vector2f(0, window->getSize().y - 3 * tileMax));
}

int Game::updatePause(){
    
    return 0;
}

int Game::update(){

    if (explosion.is_displayed){
        sf::sleep(sf::milliseconds(10));
        if (!explosion.update()){
            explosion.is_displayed = false;
        }
    }

    if (gameover){
        return -1;
    }

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

    sound = maxSound(sound, playerFall(p1));
    if (!solo)
        sound = maxSound(sound, playerFall(p2));

    if (sound == SoundType::BUMP)
        bumpSound.play();
    else if (sound == SoundType::BIM)
        bimSound.play();
    else if (sound == SoundType::EXPLOSION)
        explosionSound.play();    
    
    Player *deadPlayer = checkDeath();
    if (deadPlayer != NULL)
        gameOver(deadPlayer);

    hpBar1.move(sf::Vector2f(0, -speed));
    hpBar1.setTexture(hpBar1.textures[p1.getLife()]);
    hpBar2.move(sf::Vector2f(0, -speed));
    if (!solo)
        hpBar2.setTexture(hpBar2.textures[p2.getLife()]);
    else
        hpBar2.setTexture(hpBar2.textures[0]);

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


bool Game::updateTile(){
    tileProgress += speed;
    if (tileProgress >= tileMax){
        tileProgress -= tileMax;
        return true;
    }
    return false;
}


int Game::tileChange(){

    (*currentTile).move(sf::Vector2f(0, -3 * tileMax));

    std::string roadString = "road_";
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
            roadString += "3_to_";
            if (r < changeChance / 2){
                roadString += "2_right";
                newRoads = { false, false, true, true, false };
                transitionRoads.assign(newRoads.begin(), newRoads.end());
            }
            else{
                roadString += "2_left";
                newRoads = { false, true, true, false, false };
                transitionRoads.assign(newRoads.begin(), newRoads.end());
            }
        }
        else if (areEqual(oldTileRoads, std::vector<bool>{ false, false, true, true, false })){
            roadString += "2_right_to_3";
            newRoads = { false, false, true, true, false };
            transitionRoads = { false, true, true, true, false };
        }
        else if (areEqual(oldTileRoads, std::vector<bool>{ false, true, true, false, false })){
            roadString += "2_left_to_3";
            newRoads = { false, true, true, false, false };
            transitionRoads = { false, true, true, true, false };
        }
        else{
            //TODO
        }
    }
    else{
        if (areEqual(oldTileRoads, std::vector<bool>{ false, true, true, true, false })){
            roadString += "3";
            newRoads = { false, true, true, true, false };
        }
        else if (areEqual(oldTileRoads, std::vector<bool>{ false, false, true, true, false })){
            roadString += "2_right";
            newRoads = { false, false, true, true, false };
        }
        else if (areEqual(oldTileRoads, std::vector<bool>{ false, true, true, false, false })){
            roadString += "2_left";
            newRoads = { false, true, true, false, false };
        }
        else{
            //TODO
        }
        transitionRoads = { false, false, false, false, false};
    }

    currentTile->setRoadNb(roadString);
    currentTile->setRoads(newRoads);
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
    if (p1.getLife() <= 0)
        return &p1;
    else if (!solo && (p2.getLife() <= 0))
        return &p2;
    return NULL;
}

SoundType Game::playerFall(Player &p){
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
    if (p.getPosition().x < xMin){
        return p.bumpBarrier(sf::Vector2f(xMin-barrierPush, p.getPosition().y));
    }
    else if (p.getPosition().x > xMax){
        return p.bumpBarrier(sf::Vector2f(xMax+barrierPush, p.getPosition().y));
    }
    return SoundType::NONE;
}

int Game::gameOver(Player *deadPlayer){

    gameover = true;
    music.stop();
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
        if (!gameover)
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
