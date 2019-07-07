#include "ServiceLocator.h"
#include "NullAudio.h"
#include "NullTextureService.h"

std::unique_ptr<AudioService> ServiceLocator::mAudioService;
std::unique_ptr<TextureService> ServiceLocator::mTextureService;

ServiceLocator::ServiceLocator()
{
    ServiceLocator::provideAudioService(nullptr);
    ServiceLocator::provideTextureService(nullptr);
}


ServiceLocator::~ServiceLocator()
{
}

AudioService * ServiceLocator::getAudioService()
{
    return ServiceLocator::mAudioService.get();
}

TextureService * ServiceLocator::getTextureService()
{
    return ServiceLocator::mTextureService.get();
}

void ServiceLocator::provideAudioService(AudioService * service)
{
    if (service == nullptr)
    {
        mAudioService.reset(new NullAudioService());
    }
    else
    {
        mAudioService.reset(service);
    }
}

void ServiceLocator::provideTextureService(TextureService * service)
{
    if (service == nullptr)
    {
        mTextureService.reset(new NullTextureService());
    }
    else
    {
        mTextureService.reset(service);
    }
}
