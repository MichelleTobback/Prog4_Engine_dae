#pragma once
#include "Audio/SoundSystem.h"

#include <memory>

namespace dae
{
	class SdlSoundSystem final : public SoundSystem
	{
	public:
		SdlSoundSystem();
		virtual ~SdlSoundSystem() override;

		virtual SoundId AddSound(const std::string& filepath) override;
		virtual void Play(SoundId id, float volume, int loop = 0) override;
		virtual void PlayForDuration(SoundId sound, float volume, float duration) override;

		virtual SoundId AddMusic(const std::string& filepath) override;
		virtual void PlayMusic(SoundId id, float volume, int loop = 0) override;
		virtual void PlayMusicForDuration(SoundId music, float volume, float duration) override;
		virtual void StopMusic() override;
		virtual void PauseMusic() override;

		virtual void SetMasterVolume(float volume) override;
		virtual float GetMasterVolume() const override;

	private:
		class SdlSoundSystemImpl;
		std::unique_ptr<SdlSoundSystemImpl> m_pImpl;
	};
}