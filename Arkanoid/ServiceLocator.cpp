#include "ServiceLocator.h"
#include "NullAudio.h"

std::unique_ptr<AudioService> ServiceLocator::mAudioService;

ServiceLocator::ServiceLocator()
{
    ServiceLocator::provide(nullptr);
}


ServiceLocator::~ServiceLocator()
{
}

AudioService * ServiceLocator::getAudio()
{
    return ServiceLocator::mAudioService.get();
}

void ServiceLocator::provide(AudioService * service)
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
