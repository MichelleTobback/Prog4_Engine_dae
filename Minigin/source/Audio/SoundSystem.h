#pragma once
#include <string>

namespace dae
{
	using SoundId = unsigned short;
	constexpr SoundId INVALID_SOUND_ID = UINT16_MAX;
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual SoundId AddSound(const std::string& filepath) = 0;
		virtual void Play(SoundId sound, float volume, int loop = 0) = 0;
		virtual void PlayForDuration(SoundId sound, float volume, float duration) = 0;

		virtual SoundId AddMusic(const std::string& filepath) = 0;
		virtual void PlayMusic(SoundId id, float volume, int loop = 0) = 0;
		virtual void PlayMusicForDuration(SoundId sound, float volume, float duration) = 0;
		virtual void StopMusic() = 0;
		virtual void PauseMusic() = 0;

		virtual void SetMasterVolume(float volume) = 0;
		virtual float GetMasterVolume() const = 0;

	private:

	};

	class NullSoundSystem final : public SoundSystem
	{
	public:
		virtual ~NullSoundSystem() = default;
		virtual SoundId AddSound(const std::string&) override { return 0u; }
		virtual void Play(SoundId, float, int = 0) override {}
		virtual void PlayForDuration(SoundId, float, float) override {}

		virtual SoundId AddMusic(const std::string&) override { return 0u; }
		virtual void PlayMusic(SoundId, float, int = 0) override {}
		virtual void PlayMusicForDuration(SoundId, float, float) override {}
		virtual void StopMusic() override {}
		virtual void PauseMusic() override {}

		virtual void SetMasterVolume(float) override {}
		virtual float GetMasterVolume() const override { return 0.f; }
	};
}