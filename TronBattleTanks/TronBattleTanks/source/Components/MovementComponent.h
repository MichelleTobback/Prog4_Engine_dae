#pragma once
#include "Component/Component.h"

#include "Core/Math.h"

namespace dae
{
	class RigidBody2DComponent;
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

		virtual void Awake() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;

		void Move(const glm::vec2& dir);

		inline void SetMovementSpeed(float speed) { m_MovementSpeed = speed; }
		inline float GetMovementSpeed() const { return m_MovementSpeed; }
		inline const glm::vec2& GetCurrentDirection() const { return m_CurrentMovementDirection; }
		inline RigidBody2DComponent& GetRigidBody() const { return *m_pRigidBody; }

	private:
		float m_MovementSpeed{ 150.f };
		float m_Acceleration{ 0.3f };
		float m_Deceleration{ 0.9f };

		glm::vec2 m_CurrentMovementDirection{};

		TransformComponent* m_pTransform{ nullptr };
		RigidBody2DComponent* m_pRigidBody{ nullptr };
	};
}