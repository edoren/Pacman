#include "Sound.hpp"

Sounds::Sound::Sound(std::string soundFile) {
    this->buffer.loadFromFile(soundFile);
    this->setBuffer(this->buffer);
}

Sounds::Sounds() : intro("resources/sounds/Start Game.wav"),
                   lose("resources/sounds/Pacman lose.wav"),
                   siren("resources/sounds/Siren.wav") {
    this->siren.setLoop(true);
    this->siren.setVolume(70.f);
}

void Sounds::stopSounds() {
    this->intro.stop();
    this->siren.stop();
    this->lose.stop();
}
