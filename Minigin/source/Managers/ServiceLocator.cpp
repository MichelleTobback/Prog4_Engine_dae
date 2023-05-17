#include "ServiceLocator.h"

std::unique_ptr<dae::SoundSystem> dae::ServiceLocator::s_SoundSystemInstance{std::make_unique<NullSoundSystem>()};

dae::SoundSystem& dae::ServiceLocator::GetSoundSystem()
{
    return *s_SoundSystemInstance;
}

void dae::ServiceLocator::RegisterSoundSystem(std::unique_ptr<SoundSystem>&& soundSystem)
{
    s_SoundSystemInstance = (soundSystem == nullptr) ? std::make_unique<NullSoundSystem>() : std::move(soundSystem);
}
