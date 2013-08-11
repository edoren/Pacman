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

public:
    Sounds();

    Sound intro;
    Sound lose;
    Sound siren;

    void stopSounds();
};


#endif // __SOUND_H__
