#ifndef SERVICE_LOCATOR_H
#define SERVICE_LOCATOR_H

#include "AudioService.h"
#include "TextureService.h"

#include <memory>

/// <summary>
/// Acts as a single place to access services.
/// </summary>
class ServiceLocator
{
public:
    ServiceLocator();
    ~ServiceLocator();

    static AudioService* getAudioService();
    static TextureService* getTextureService();

    static void provideAudioService(AudioService* service);
    static void provideTextureService(TextureService* service);

private:
    static std::unique_ptr<AudioService> mAudioService;
    static std::unique_ptr<TextureService> mTextureService;
};


#endif // !SERVICE_LOCATOR_H
