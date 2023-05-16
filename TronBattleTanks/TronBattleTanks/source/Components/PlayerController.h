#pragma once
#include "Component/Component.h"

#include "Core/Math.h"

namespace dae
{
	class MovementComponent;
	class TransformComponent;
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

		virtual void Update() override;

		void Attack();
		void DealDamage(float damage);
		void Reset();

		void Move(const glm::vec2& dir);
		void Rotate(float dir);

	private:
		int m_ControllerIndex;
		float m_RotationSpeed{ 45.f };

		HealthComponent* m_pHealth{nullptr};
		MovementComponent* m_pMovement{ nullptr };

		GameObject* m_pTankCanon{};
	};
}