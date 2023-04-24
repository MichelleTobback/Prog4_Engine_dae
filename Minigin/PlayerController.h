#pragma once
#include "Component.h"

namespace dae
{
	class HealthComponent;
	class PlayerController : public Component
	{
	public:
		PlayerController(GameObject* pOwner, int controllerIndex = -1);
		virtual ~PlayerController() override = default;

		PlayerController(const PlayerController& other) = delete;
		PlayerController(PlayerController&& other) = delete;
		PlayerController& operator=(const PlayerController& other) = delete;
		PlayerController& operator=(PlayerController&& other) = delete;

		void Attack();
		void DealDamage(float damage);
		void SetOpponent(PlayerController* pOpponent);
		void Reset();

	private:
		int m_ControllerIndex;

		HealthComponent* m_pHealth{nullptr};
		PlayerController* m_pOpponent{ nullptr };
	};
}