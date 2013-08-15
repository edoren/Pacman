#ifndef SOUND_HPP
#define SOUND_HPP

#include <SFML/Audio.hpp>
#include "Config.hpp"

class Sounds {
private:
    class Sound : public sf::Sound {
        sf::SoundBuffer buffer;
    public:
        Sound(std::string soundFile);
    };

    bool chomp;
    Sound chomp1;
    Sound chomp2;

public:
    Sounds();

    Sound intro;
    Sound lose;
    Sound siren;

    void stopSounds();
    void playChomp();
};

#endif // SOUND_HPP
