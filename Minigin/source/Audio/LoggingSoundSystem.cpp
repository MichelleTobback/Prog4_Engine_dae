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

void dae::LoggingSoundSystem::Play(SoundId id, float volume, int loop)
{
	m_pRealSoundSystem->Play(id, volume, loop);
	std::cout << "Play sound with ID : " << id << " at volume " << volume << '\n';
}

void dae::LoggingSoundSystem::PlayForDuration(SoundId id, float volume, float duration)
{
	m_pRealSoundSystem->PlayForDuration(id, volume, duration);
	std::cout << "Play sound with ID : " << id << " at volume " << volume << " for " << duration << "s\n";
}

dae::SoundId dae::LoggingSoundSystem::AddMusic(const std::string& filepath)
{
	SoundId id{ m_pRealSoundSystem->AddMusic(filepath) };
	std::cout << "Registered music from path : " << filepath << " to ID " << id << '\n';
	return id;
}

void dae::LoggingSoundSystem::PlayMusic(SoundId id, float volume, int loop)
{
	m_pRealSoundSystem->PlayMusic(id, volume, loop);
	std::cout << "Play music with ID : " << id << " at volume " << volume << '\n';
}

void dae::LoggingSoundSystem::PlayMusicForDuration(SoundId id, float volume, float duration)
{
	m_pRealSoundSystem->PlayMusicForDuration(id, volume, duration);
	std::cout << "Play music with ID : " << id << " at volume " << volume << " for " << duration << "s\n";
}

void dae::LoggingSoundSystem::StopMusic()
{
	m_pRealSoundSystem->StopMusic();
	std::cout << "Stop playing music\n";
}

void dae::LoggingSoundSystem::PauseMusic()
{
	m_pRealSoundSystem->PauseMusic();
	std::cout << "Pause Music\n";
}

void dae::LoggingSoundSystem::SetMasterVolume(float volume)
{
	std::cout << "Set volume from " << m_pRealSoundSystem->GetMasterVolume();
	m_pRealSoundSystem->SetMasterVolume(volume);
	std::cout << " to " << m_pRealSoundSystem->GetMasterVolume() << '\n';
}

float dae::LoggingSoundSystem::GetMasterVolume() const
{
	return m_pRealSoundSystem->GetMasterVolume();
}
