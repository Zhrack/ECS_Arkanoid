#include "PCAudioService.h"

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <string>
#include <iostream>

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

PCAudioService::PCAudioService() :
    AudioService()
{
}


PCAudioService::~PCAudioService()
{
    for (auto m : mMusics)
    {
        delete m.second;
    }
}

bool PCAudioService::initialize(const std::string& sfxFolder, const std::string& musicFolder, 
    const std::vector<SoundIDFilename>& sfxFiles, const std::vector<MusicIDFilename>& musicFiles)
{
    try
    {
        fs::path audioPath(sfxFolder);
        if (fs::is_directory(audioPath)) {

            // preallocate all needed elements, this way I can access them by index
            mSoundBuffers.clear();
            mSoundBuffers.insert(mSoundBuffers.end(), sfxFiles.size(), sf::SoundBuffer());

            for (auto pair : sfxFiles)
            {
                fs::path filePath(sfxFolder + pair.second);
                if (!fs::is_regular_file(filePath))
                {
                    throw "Error opening " + filePath.filename().string();
                }

                // SoundID is used both as enumerator for outside use and as index for fast retrieval
                sf::SoundBuffer& buffer = mSoundBuffers[pair.first];

                if (!buffer.loadFromFile(filePath.string()))
                {
                    throw "Error loading " + filePath.string();
                }
            }
        }
        else
        {
            throw "Invalid SFX path " + audioPath.string();
        }

        fs::path musicPath(musicFolder);

        if (fs::is_directory(musicPath)) {

            // preallocate all needed elements, this way I can access them by index
            mMusics.clear();

            for (auto pair : musicFiles)
            {
                fs::path filePath(musicFolder + pair.second);
                if (!fs::is_regular_file(filePath))
                {
                    throw "Error opening " + filePath.filename().string();
                }

                // SoundID is used both as enumerator for outside use and as index for fast retrieval
                sf::Music* music = new sf::Music();

                if (!music->openFromFile(filePath.string()))
                {
                    throw "Error loading " + filePath.string();
                }

                mMusics[pair.first] = music;
            }
        }
        else
        {
            throw "Invalid music path " + musicPath.string();
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;

        return false;
    }

    return true;
}

sf::SoundBuffer* PCAudioService::getSound(SoundID soundID)
{
    return &(mSoundBuffers[soundID]);
}

void PCAudioService::setVolume(MusicID soundID, float volume)
{
    mMusics[soundID]->setVolume(volume);
}

void PCAudioService::playMusic(MusicID soundID)
{
    mMusics[soundID]->play();
}

void PCAudioService::stopMusic(MusicID soundID)
{
    mMusics[soundID]->stop();
}

void PCAudioService::setLooping(MusicID soundID, bool looping)
{
    mMusics[soundID]->setLoop(looping);
}