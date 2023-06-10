#include "MovementComponent.h"
#include "Core/Time.h"
#include "Component/TransformComponent.h"
#include "Scene/GameObject.h"
#include "Component/Physics/RigidBody2DComponent.h"

dae::MovementComponent::MovementComponent(GameObject* pOwner)
	: Component(pOwner)
{
	m_pTransform = &pOwner->GetTransform();
	m_pRigidBody = GetOwner()->AddComponent<RigidBody2DComponent>();
}

void dae::MovementComponent::Awake()
{
	
}

void dae::MovementComponent::Update()
{
	const float epsilon{ 0.001f };
	//const float elapsed{ Time::GetInstance().GetDeltaTime() };
	if (std::abs(m_CurrentMovementDirection.x) > epsilon || std::abs(m_CurrentMovementDirection.y) > epsilon)
	{
		glm::vec3 vel{ glm::vec3{m_CurrentMovementDirection * m_MovementSpeed, 0.f} };
		m_pRigidBody->SetVelociy(vel);
		m_CurrentMovementDirection = {};
		return;
	}
	m_pRigidBody->SetVelociy({});
}

void dae::MovementComponent::FixedUpdate()
{
}

void dae::MovementComponent::Move(const glm::vec2& dir)
{
	m_CurrentMovementDirection = dir;
	m_CurrentMovementDirection = glm::normalize(m_CurrentMovementDirection);
}
