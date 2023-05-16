#pragma once
#include "Component/Component.h"

#include "Core/Math.h"

namespace dae
{
	class TransformComponent;
	class MovementComponent : public Component
	{
	public:
		MovementComponent(GameObject* pOwner);
		virtual ~MovementComponent() override = default;

		MovementComponent(const MovementComponent& other) = delete;
		MovementComponent(MovementComponent&& other) = delete;
		MovementComponent& operator=(const MovementComponent& other) = delete;
		MovementComponent& operator=(MovementComponent&& other) = delete;

		virtual void Update() override;

		void Move(const glm::vec2& dir);

		inline void SetMovementSpeed(float speed) { m_MovementSpeed = speed; }
		inline float GetMovementSpeed() const { return m_MovementSpeed; }
		inline const glm::vec2& GetCurrentDirection() const { return m_CurrentMovementDirection; }
		inline const glm::vec2& GetVelocity() const { return m_CurrentVelocity; }

	private:
		float m_MovementSpeed{ 20.f };
		float m_Acceleration{ 0.3f };
		float m_Deceleration{ 50.f };

		glm::vec2 m_CurrentMovementDirection{};
		glm::vec2 m_CurrentVelocity{};

		TransformComponent* m_pTransform{ nullptr };
	};
}