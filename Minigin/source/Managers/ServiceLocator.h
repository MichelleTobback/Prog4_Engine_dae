#pragma once
#include "Audio/SoundSystem.h"

#include <memory>

namespace dae
{
	class ServiceLocator final
	{
	public:
		static SoundSystem& GetSoundSystem();
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& soundSystem);

	private:
		static std::unique_ptr<SoundSystem> s_SoundSystemInstance;
	};
}