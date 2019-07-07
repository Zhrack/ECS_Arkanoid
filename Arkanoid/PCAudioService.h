#ifndef PC_AUDIO_SERVICE_H
#define PC_AUDIO_SERVICE_H

#include "AudioService.h"

#include <SFML/Audio.hpp>

#include <vector>
#include <unordered_map>
#include <memory>

using SoundIDFilename = std::pair<SoundID, std::string>;
using MusicIDFilename = std::pair<MusicID, std::string>;

/// <summary>
/// Implementation of the Audio Service for PC.
/// </summary>
/// <seealso cref="AudioService" />
class PCAudioService :
    public AudioService
{
public:
    PCAudioService();
    virtual ~PCAudioService();

    bool initialize(const std::string& sfxFolder, const std::string& musicFolder, 
        const std::vector<SoundIDFilename>& sfxFiles, const std::vector<MusicIDFilename>& musicFiles);

    // Inherited via AudioService
    virtual sf::SoundBuffer* getSound(SoundID soundID) override;
    virtual void setVolume(MusicID soundID, float volume) override;
    virtual void playMusic(MusicID soundID) override;
    virtual void stopMusic(MusicID soundID) override;
    virtual void setLooping(MusicID soundID, bool looping) override;

private:
    // where the data is
    std::vector<sf::SoundBuffer> mSoundBuffers;

    /// <summary>
    /// Musics of the game. Is a pointer because sf::Music has a deleted copy constructor
    /// </summary>
    std::unordered_map<MusicID,sf::Music*> mMusics;
};


#endif // !PC_AUDIO_SERVICE_H
