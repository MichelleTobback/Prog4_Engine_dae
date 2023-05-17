#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <chrono>
#include "Minigin.h"
#include "Input/Input.h"
#include "Managers/SceneManager.h"
#include "Renderer/Renderer.h"
#include "Managers/ResourceManager.h"
#include "Managers/ServiceLocator.h"
#include "Platform/SDL/SdlSoundSystem.h"
#include "Platform/SDL/SdlWindow.h"

#if _DEBUG
	#include "Audio/LoggingSoundSystem.h"
#endif

#include "Time.h"

dae::Minigin::Minigin(const std::string &dataPath)
{
	ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	ServiceLocator::GetWindow().Shutdown();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
#if _DEBUG
	ServiceLocator::RegisterSoundSystem(std::make_unique<LoggingSoundSystem>(std::make_unique<SdlSoundSystem>()));
#else
	ServiceLocator::RegisterSoundSystem(std::make_unique<SdlSoundSystem>());
#endif

	WindowDesc windowSpecs{};
	windowSpecs.title = "Minigin";
	ServiceLocator::RegisterWindow(std::make_unique<SdlWindow>(windowSpecs));

	auto& window{ ServiceLocator::GetWindow() };
	window.Init();
	Renderer::GetInstance().Init(window);

	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = Input::GetInstance();

	// todo: this update loop could use some work.
	dae::Time& time{ Time::GetInstance() };
	float lag{};
	bool doContinue = true;
	time.Start();
	while (doContinue)
	{
		time.Update();
		doContinue = input.ProcessInput();
		input.HandleInput();
		sceneManager.Update();

		lag += time.GetDeltaTime();

		while (lag >= time.GetFixedTimeStep())
		{
			sceneManager.FixedUpdate();
			lag -= time.GetFixedTimeStep();
		}

		sceneManager.LateUpdate();
		renderer.Render();
		sceneManager.HandleObjectsLifeTime();

		time.Wait();
	}
}
