#pragma once
#include "SoundSystem.h"

#include <memory>

namespace dae
{
	class LoggingSoundSystem final : public SoundSystem
	{
	public:
		LoggingSoundSystem(std::unique_ptr<SoundSystem>&& soundSystem);
		virtual ~LoggingSoundSystem() = default;

		virtual SoundId AddSound(const std::string& path) override;
		virtual void Play(SoundId id, float volume, int loop = 0) override;
		virtual void PlayForDuration(SoundId sound, float volume, float duration) override;

		virtual SoundId AddMusic(const std::string& filepath) override;
		virtual void PlayMusic(SoundId id, float volume, int loop = 0) override;
		virtual void PlayMusicForDuration(SoundId sound, float volume, float duration) override;
		virtual void StopMusic() override;
		virtual void PauseMusic() override;

		virtual void SetMasterVolume(float volume) override;
		virtual float GetMasterVolume() const override;

	private:
		std::unique_ptr<SoundSystem> m_pRealSoundSystem;
	};
}