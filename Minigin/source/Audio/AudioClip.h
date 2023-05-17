#pragma once
#include "Audio/SoundSystem.h"

namespace dae
{
	class AudioClip final
	{
	public:
		AudioClip(SoundId id = INVALID_SOUND_ID, float volume = 90.f);
		AudioClip(const std::string& filepath);

		void Load(const std::string& filepath);
		void SetVolume(float volume);
		void Play();
		void Play(float volume);

	private:
		SoundId m_Id;
		float m_Volume;
	};
}