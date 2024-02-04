#include "events.hpp"

/*This is used to play the background music. It is as simple as
    startMusic();
    stopMusic();
To be included in the main, or when a key is pressed ??
*/
void startMusic() {
    const std::string filename = "../sound/ambiant_music.m4a";

    if (!music.openFromFile(filename)) {
        std::cerr << "Failed to load music file: " << filename << std::endl;
        return;
    }

    music.play();
}

void stopMusic() {
    music.stop();
}



void playBump(){
    /*Cette fonction joue un son de choc lorsquelle est appelée*/
    sf::SoundBuffer buffer;
    const std::string filePath = "../sound/bump.wav";
    if (!buffer.loadFromFile(filePath)) {
        std::cerr << "Erreur lors du chargement du fichier audio." << std::endl;
        return;
    }

    sf::Sound sound;
    sound.setBuffer(buffer);

    sound.play();
}

void showExplosion(sf::RenderWindow& window, const sf::Vector2f& position) {
    // Load explosion textures
    sf::Texture explosionTextures[4];
    for (int i = 0; i < 4; ++i) {
        if (!explosionTextures[i].loadFromFile("../animations/explsion/explosion_frame" + std::to_string(i) + ".png")) {
            std::cerr << "Failed to load explosion texture." << std::endl;
            return;
        }
    }

    sf::Sprite explosionSprite(explosionTextures[0]);
    explosionSprite.setPosition(position);

    const float animationSpeed = 0.1f;

    sf::Clock animationClock;
    while (animationClock.getElapsedTime().asSeconds() < animationSpeed) {
        for (int i = 0; i < 4; ++i) {
            explosionSprite.setTexture(explosionTextures[i]);
            window.draw(explosionSprite);
            window.display();
            sf::sleep(sf::seconds(0.1f)); // The sleep duration can be adjusted
        }
    }
}


