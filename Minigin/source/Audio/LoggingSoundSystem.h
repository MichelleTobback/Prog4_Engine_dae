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
		virtual void Play(SoundId id, float volume);

	private:
		std::unique_ptr<SoundSystem> m_pRealSoundSystem;
	};
}