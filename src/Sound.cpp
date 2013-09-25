#include "Sound.hpp"

Sounds::Sound::Sound(std::string soundFile) {
    this->buffer.loadFromFile(soundFile);
    this->setBuffer(this->buffer);
}

Sounds::Sounds() : intro(pacmanPath + "resources/sounds/Start Game.wav"),
                   lose(pacmanPath + "resources/sounds/Pacman lose.wav"),
                   siren(pacmanPath + "resources/sounds/Siren.wav"),
                   chomp1(pacmanPath + "resources/sounds/Chomp1.wav"),
                   chomp2(pacmanPath + "resources/sounds/Chomp2.wav"),
                   intermission(pacmanPath + "resources/sounds/Intermission.wav") {
    this->siren.setLoop(true);

    this->intro.setVolume(60.f);
    this->lose.setVolume(60.f);
    this->siren.setVolume(40.f);
    this->chomp1.setVolume(35.f);
    this->chomp2.setVolume(35.f);
    this->intermission.setVolume(35.f);

    this->chomp = true;
}

void Sounds::stopSounds() {
    this->intro.stop();
    this->siren.stop();
    this->lose.stop();
    this->chomp1.stop();
    this->chomp2.stop();
    this->intermission.stop();
}

void Sounds::playChomp() {
    if(chomp) chomp1.play();
    else chomp2.play();
    chomp = !chomp;
}
