#pragma once
#include "Component/Component.h"

#include <glm/glm.hpp>

namespace dae
{
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

		void Attack();
		void DealDamage(float damage);
		void Reset();

		void Move(const glm::vec2& dir);

	private:
		int m_ControllerIndex;
		float m_MovementSpeed{ 2.f };

		HealthComponent* m_pHealth{nullptr};
		TransformComponent* m_pTransform{ nullptr };
	};
}