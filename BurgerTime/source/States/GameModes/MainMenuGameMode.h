#pragma once
#include "State/GameState/GameState.h"

namespace dae
{
	class MainMenuGameMode final : public GameMode
	{
	public:
		MainMenuGameMode();
		virtual ~MainMenuGameMode() = default;

		MainMenuGameMode(const MainMenuGameMode& other) = delete;
		MainMenuGameMode(MainMenuGameMode&& other) = delete;
		MainMenuGameMode& operator=(const MainMenuGameMode& other) = delete;
		MainMenuGameMode& operator=(MainMenuGameMode&& other) = delete;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	private:
		void OnSceneLoaded(Scene& scene);
		void OnSinglePlayerModeSelected();
		void OnVersusModeSelected();
		void OnCoOpModeSelected();

		size_t m_SceneIndex{};
	};
}