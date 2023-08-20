#pragma once
#include "States/GameStates/BTGameMode.h"

namespace dae
{
	class BTVersusGameMode final : public BTGameMode
	{
	public:
		BTVersusGameMode(size_t lvlIdx);
		virtual ~BTVersusGameMode() = default;

		BTVersusGameMode(const BTVersusGameMode& other) = delete;
		BTVersusGameMode(BTVersusGameMode&& other) = delete;
		BTVersusGameMode& operator=(const BTVersusGameMode& other) = delete;
		BTVersusGameMode& operator=(BTVersusGameMode&& other) = delete;

		virtual void OnEnter(Scene& scene) override;
		virtual void OnExit(Scene& scene) override;

	protected:
		virtual void StartRound() override;
		virtual GameObject* CreateHUD(Scene& scene) override;
		virtual GameObject* CreatePlayer(dae::Scene* pScene) override;
	};
}