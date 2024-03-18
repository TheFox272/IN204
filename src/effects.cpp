#include "effects.hpp"

/*----------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------*/

bool Explosion::update(){
    if (frame < 63)
    {
        frame++;
        setTexture(textures[frame]);
        return true;
    }
    return false;
}

void startMusic(sf::Music& music) {
    const std::string filename = "../sound/ambiant.ogg";

    if (!music.openFromFile(filename)) {
        std::cerr << "Failed to load music file: " << filename << std::endl;
        return;
    }
    music.setLoop(true);
    music.play();
}

void stopMusic(sf::Music& music) {
    music.stop();
}

void initializeBump(sf::SoundBuffer& buffer, sf::Sound& sound){
    const std::string filePath = "../sound/bump.wav";
    if (!buffer.loadFromFile(filePath)) {
        std::cerr << "Erreur lors du chargement du fichier audio." << std::endl;
        return;
    }

    sound.setBuffer(buffer);
}

void initializeBim(sf::SoundBuffer& buffer, sf::Sound& sound){
    const std::string filePath = "../sound/Explosion 1.wav";
    if (!buffer.loadFromFile(filePath)) {
        std::cerr << "Erreur lors du chargement du fichier audio." << std::endl;
        return;
    }

    sound.setBuffer(buffer);
}

void initializeExplosion(sf::SoundBuffer& buffer, sf::Sound& sound){
    const std::string filePath = "../sound/Explosion 5.wav";
    if (!buffer.loadFromFile(filePath)) {
        std::cerr << "Erreur lors du chargement du fichier audio." << std::endl;
        return;
    }

    sound.setBuffer(buffer);
}
