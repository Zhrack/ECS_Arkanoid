#ifndef NULL_AUDIO_H
#define NULL_AUDIO_H

#include "AudioService.h"

#include <SFML/Audio.hpp>

/// <summary>
/// To disable sound. Useful for development.
/// </summary>
/// <seealso cref="AudioService" />
class NullAudioService :
    public AudioService
{
public:
    NullAudioService();
    virtual ~NullAudioService();

    // Inherited via AudioService
    virtual sf::SoundBuffer * getSound(SoundID soundID) override;
    virtual void setVolume(MusicID soundID, float volume) override;
    virtual void playMusic(MusicID soundID) override;
    virtual void stopMusic(MusicID soundID) override;
    virtual void setLooping(MusicID soundID, bool looping) override;
};

#endif // !NULL_AUDIO_H
