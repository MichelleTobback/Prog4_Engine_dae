#include "AudioClip.h"
#include "Managers/ServiceLocator.h"

dae::AudioClip::AudioClip(SoundId id, float volume)
	: m_Id{id}, m_Volume{ volume }
{
}

dae::AudioClip::AudioClip(const std::string& filepath)
	: AudioClip()
{
	Load(filepath);
}

void dae::AudioClip::Load(const std::string& filepath)
{
	m_Id = ServiceLocator::GetSoundSystem().AddSound(filepath);
}

void dae::AudioClip::SetVolume(float volume)
{
	m_Volume = volume;
}

void dae::AudioClip::Play()
{
	ServiceLocator::GetSoundSystem().Play(m_Id, m_Volume);
}

void dae::AudioClip::Play(float volume)
{
	ServiceLocator::GetSoundSystem().Play(m_Id, volume);
}
