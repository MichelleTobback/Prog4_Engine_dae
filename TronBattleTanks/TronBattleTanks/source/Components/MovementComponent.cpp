#include "MovementComponent.h"
#include "Core/Time.h"
#include "Component/TransformComponent.h"
#include "Scene/GameObject.h"

dae::MovementComponent::MovementComponent(GameObject* pOwner)
	: Component(pOwner)
{
	m_pTransform = &pOwner->GetTransform();
}

void dae::MovementComponent::Update()
{
	float dt{ Time::GetInstance().GetDeltaTime() };
	glm::vec2 deltaVel{ m_CurrentMovementDirection * dt * m_MovementSpeed };
	m_CurrentVelocity += deltaVel;
	float currentSpeed{ glm::length(m_CurrentVelocity) };
	if (currentSpeed > m_MovementSpeed)
	{
		glm::vec2 direction{ glm::normalize(m_CurrentVelocity) };
		m_CurrentVelocity = direction * m_MovementSpeed;
	}

	m_pTransform->Translate({ m_CurrentVelocity.x, m_CurrentVelocity.y, 0.f });

	if (currentSpeed > 0.1f)
	{
		m_CurrentVelocity -= m_CurrentVelocity * m_Deceleration * dt;
	}
	else
	{
		m_CurrentVelocity = {};
	}
	m_CurrentMovementDirection = {};
}

void dae::MovementComponent::Move(const glm::vec2& dir)
{
	m_CurrentMovementDirection += dir;
	m_CurrentMovementDirection = glm::normalize(m_CurrentMovementDirection);
}
