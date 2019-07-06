#include "NullAudio.h"

NullAudioService::NullAudioService()
{
}

NullAudioService::~NullAudioService()
{
}

sf::SoundBuffer * NullAudioService::getSound(SoundID soundID)
{
    soundID;
    return nullptr;
}

void NullAudioService::setVolume(MusicID soundID, float volume)
{
    soundID; volume;
}

void NullAudioService::playMusic(MusicID soundID)
{
    soundID;
}

void NullAudioService::stopMusic(MusicID soundID)
{
    soundID;
}

void NullAudioService::setLooping(MusicID soundID, bool looping)
{
    soundID; looping;
}
