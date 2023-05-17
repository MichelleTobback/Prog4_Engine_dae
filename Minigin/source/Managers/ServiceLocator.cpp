#include "ServiceLocator.h"

std::unique_ptr<dae::SoundSystem> dae::ServiceLocator::s_pSoundSystemInstance{std::make_unique<NullSoundSystem>()};
std::unique_ptr<dae::Window> dae::ServiceLocator::s_pWindow{ std::make_unique<NullWindow>() };

dae::SoundSystem& dae::ServiceLocator::GetSoundSystem()
{
    return *s_pSoundSystemInstance;
}

void dae::ServiceLocator::RegisterSoundSystem(std::unique_ptr<SoundSystem>&& pSoundSystem)
{
    s_pSoundSystemInstance = (pSoundSystem == nullptr) ? std::make_unique<NullSoundSystem>() : std::move(pSoundSystem);
}

dae::Window& dae::ServiceLocator::GetWindow()
{
    return *s_pWindow;
}

void dae::ServiceLocator::RegisterWindow(std::unique_ptr<Window>&& pWindow)
{
    s_pWindow = (pWindow == nullptr) ? std::make_unique<NullWindow>() : std::move(pWindow);
}
