#ifndef SERVICE_LOCATOR_H
#define SERVICE_LOCATOR_H

#include "AudioService.h"

#include <memory>

class ServiceLocator
{
public:
    ServiceLocator();
    ~ServiceLocator();

    static AudioService* getAudio();

    static void provide(AudioService* service);

private:
    static std::unique_ptr<AudioService> mAudioService;
};


#endif // !SERVICE_LOCATOR_H
