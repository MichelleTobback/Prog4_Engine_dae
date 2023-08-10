#pragma once
#include "States/GameModes/BTGameMode.h"

namespace dae
{
	class BTSinglePlayerGameMode final : public BTGameMode
	{
	public:
		BTSinglePlayerGameMode(size_t level = 0);
		virtual ~BTSinglePlayerGameMode() = default;

		BTSinglePlayerGameMode(const BTSinglePlayerGameMode& other) = delete;
		BTSinglePlayerGameMode(BTSinglePlayerGameMode&& other) = delete;
		BTSinglePlayerGameMode& operator=(const BTSinglePlayerGameMode& other) = delete;
		BTSinglePlayerGameMode& operator=(BTSinglePlayerGameMode&& other) = delete;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	protected:
		virtual void StartRound() override;
		virtual GameObject* CreateHUD() override;
	};
}