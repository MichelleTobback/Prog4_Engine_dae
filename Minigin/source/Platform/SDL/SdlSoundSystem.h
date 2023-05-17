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
		virtual void Play(SoundId id, float volume) override;

		class SdlSoundSystemImpl;

	private:
		std::unique_ptr<SdlSoundSystemImpl> m_pImpl;
	};
}