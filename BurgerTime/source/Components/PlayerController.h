#pragma once
#include "Component/Component.h"
#include "BurgerTimeMovementController.h"

#include "Core/Math.h"

namespace dae
{
	constexpr CollisionLayer PLAYER_COLLISION_LAYER = CollisionLayer::Two;

	class PlayerController : public Component
	{
	public:
		PlayerController(GameObject* pOwner, BurgerTimeMovementController* pCharactarerController, int controllerIndex = -1);
		virtual ~PlayerController() override = default;

		PlayerController(const PlayerController& other) = delete;
		PlayerController(PlayerController&& other) = delete;
		PlayerController& operator=(const PlayerController& other) = delete;
		PlayerController& operator=(PlayerController&& other) = delete;

		virtual void Awake() override;
		virtual void LateUpdate() override;

	private:
		void OnDeath();

		BurgerTimeMovementController* m_pCharacterController;
		int m_ControllerIndex;
		bool m_Reset{ false };
	};
}