#pragma once
#include "Audio/SoundSystem.h"
#include "Core/Window.h"

#include <memory>

namespace dae
{
	class ServiceLocator final
	{
	public:
		static SoundSystem& GetSoundSystem();
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& pSoundSystem);

		static Window& GetWindow();
		static void RegisterWindow(std::unique_ptr<Window>&& pWindow);

	private:
		static std::unique_ptr<SoundSystem> s_pSoundSystemInstance;
		static std::unique_ptr<Window> s_pWindow;
	};
}