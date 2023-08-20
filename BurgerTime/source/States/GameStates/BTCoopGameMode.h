#pragma once
#include "States/GameStates/BTGameMode.h"

namespace dae
{
	class BTCoopGameMode final : public BTGameMode
	{
	public:
		BTCoopGameMode(size_t lvlIdx);
		virtual ~BTCoopGameMode() = default;

		BTCoopGameMode(const BTCoopGameMode& other) = delete;
		BTCoopGameMode(BTCoopGameMode&& other) = delete;
		BTCoopGameMode& operator=(const BTCoopGameMode& other) = delete;
		BTCoopGameMode& operator=(BTCoopGameMode&& other) = delete;

		virtual void OnEnter(Scene& scene) override;
		virtual void OnExit(Scene& scene) override;

	protected:
		virtual void StartRound() override;
		virtual GameObject* CreateHUD(Scene& scene) override;
		virtual GameObject* CreatePlayer(dae::Scene* pScene) override;
	};
}