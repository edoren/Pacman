#ifndef __SOUND_H__
#define __SOUND_H__

#include <SFML/Audio.hpp>

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

#endif // __SOUND_H__
