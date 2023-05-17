#include "LoggingSoundSystem.h"
#include <iostream>

dae::LoggingSoundSystem::LoggingSoundSystem(std::unique_ptr<SoundSystem>&& soundSystem)
	: m_pRealSoundSystem{std::move(soundSystem)}
{
}

dae::SoundId dae::LoggingSoundSystem::AddSound(const std::string& path)
{
	SoundId id{ m_pRealSoundSystem->AddSound(path) };

	std::cout << "Registered sound from path : " << path << " to ID " << id << '\n';

	return id;
}

void dae::LoggingSoundSystem::Play(SoundId id, float volume)
{
	m_pRealSoundSystem->Play(id, volume);
	std::cout << "Play sound with ID : " << id << " at volume " << volume << '\n';
}
