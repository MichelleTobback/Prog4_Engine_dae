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
		virtual void Play(SoundId sound, float volume) = 0;

	private:

	};

	class NullSoundSystem final : public SoundSystem
	{
	public:
		virtual ~NullSoundSystem() = default;
		virtual SoundId AddSound(const std::string&) override { return 0u; }
		virtual void Play(SoundId, float) override {}
	};
}