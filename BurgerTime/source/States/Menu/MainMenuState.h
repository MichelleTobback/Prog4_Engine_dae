#pragma once
#include "State/State.h"

namespace dae
{
	class MainMenuState final : public State
	{
	public:
		MainMenuState() = default;
		virtual ~MainMenuState() = default;

		MainMenuState(const MainMenuState& other) = delete;
		MainMenuState(MainMenuState&& other) = delete;
		MainMenuState& operator=(const MainMenuState& other) = delete;
		MainMenuState& operator=(MainMenuState&& other) = delete;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	protected:
	};
}