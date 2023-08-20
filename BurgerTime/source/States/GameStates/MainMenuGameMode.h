#pragma once
#include "States/GameStates/GameState.h"

namespace dae
{
	class Scene;
	class MainMenuGameMode final : public GameState
	{
	public:
		MainMenuGameMode();
		virtual ~MainMenuGameMode() = default;

		MainMenuGameMode(const MainMenuGameMode& other) = delete;
		MainMenuGameMode(MainMenuGameMode&& other) = delete;
		MainMenuGameMode& operator=(const MainMenuGameMode& other) = delete;
		MainMenuGameMode& operator=(MainMenuGameMode&& other) = delete;

		virtual void OnEnter(Scene& scene) override;
		virtual void OnExit(Scene& scene) override;

	private:
		void OnSinglePlayerModeSelected();
		void OnVersusModeSelected();
		void OnCoOpModeSelected();

		void CreateScene(Scene* pScene);
	};
}