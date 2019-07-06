#ifndef AUDIO_SERVICE_H
#define AUDIO_SERVICE_H

#include "AudioPCName.h"

#include <SFML/Audio.hpp>
/// <summary>
/// An interface for all AudioServices (in case you are on PC, PS4, Xbox, etc...)
/// </summary>
class AudioService
{
public:
    AudioService();
    virtual ~AudioService();
    virtual sf::SoundBuffer* getSound(SoundID soundID) = 0;

    virtual void setVolume(MusicID soundID, float volume) = 0;
    virtual void playMusic(MusicID soundID) = 0;
    virtual void stopMusic(MusicID soundID) = 0;
    virtual void setLooping(MusicID soundID, bool looping) = 0;
};


#endif // AUDIO_SERVICE_H
