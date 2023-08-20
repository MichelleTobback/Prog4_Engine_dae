#pragma once
#include "States/GameStates/BTGameMode.h"

namespace dae
{
	class BTSinglePlayerGameMode final : public BTGameMode
	{
	public:
		BTSinglePlayerGameMode(size_t lvlIdx);
		virtual ~BTSinglePlayerGameMode() = default;

		BTSinglePlayerGameMode(const BTSinglePlayerGameMode& other) = delete;
		BTSinglePlayerGameMode(BTSinglePlayerGameMode&& other) = delete;
		BTSinglePlayerGameMode& operator=(const BTSinglePlayerGameMode& other) = delete;
		BTSinglePlayerGameMode& operator=(BTSinglePlayerGameMode&& other) = delete;

		virtual void OnEnter(Scene& scene) override;
		virtual void OnExit(Scene& scene) override;

	protected:
		virtual void StartRound() override;
		virtual GameObject* CreateHUD(Scene& scene) override;
		virtual GameObject* CreatePlayer(dae::Scene* pScene) override;
	};
}