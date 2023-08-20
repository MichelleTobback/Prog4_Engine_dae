#pragma once
#include "States/GameStates/GameState.h"
#include "States/GameStates/BTGameMode.h"

namespace dae
{
	class TextComponent;
	class EnterNameMenuState final : public GameState
	{
	public:
		EnterNameMenuState(const std::shared_ptr<BTGameMode>& pGameMode);
		virtual ~EnterNameMenuState() = default;

		EnterNameMenuState(const EnterNameMenuState& other) = delete;
		EnterNameMenuState(EnterNameMenuState&& other) = delete;
		EnterNameMenuState& operator=(const EnterNameMenuState& other) = delete;
		EnterNameMenuState& operator=(EnterNameMenuState&& other) = delete;

		virtual void OnEnter(Scene& scene) override;
		virtual void OnExit(Scene& scene) override;

	private:
		std::shared_ptr<BTGameMode> m_pSelectedGameMode;
		void StartGame();
	};
}