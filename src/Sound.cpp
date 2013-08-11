#include "Sound.hpp"
#include <iostream>

Sounds::Sound::Sound(std::string soundFile) {
    this->buffer.loadFromFile(soundFile);
    this->setBuffer(this->buffer);
}

Sounds::Sounds() : intro("resources/sounds/Start Game.wav"),
                   lose("resources/sounds/Pacman lose.wav"),
                   siren("resources/sounds/Siren.wav"),
                   chomp1("resources/sounds/Chomp1.wav"),
                   chomp2("resources/sounds/Chomp2.wav") {
    this->siren.setLoop(true);
    this->siren.setVolume(70.f);
    this->chomp = true;
}

void Sounds::stopSounds() {
    this->intro.stop();
    this->siren.stop();
    this->lose.stop();
    this->chomp1.stop();
    this->chomp2.stop();
}

void Sounds::playChomp() {
    if(chomp) chomp1.play();
    else chomp2.play();
    chomp = !chomp;
}
