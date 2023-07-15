#include "CharacterController2D.h"

dae::CharacterController2D::CharacterController2D(GameObject* pOwner, const CharacterController2DDesc& desc)
	: Component(pOwner), m_Desc{desc}
{
}

void dae::CharacterController2D::Update()
{
	const float epsilon{ 0.001f };
	//const float elapsed{ Time::GetInstance().GetDeltaTime() };
	if (std::abs(m_CurrentMovementDirection.x) > epsilon || std::abs(m_CurrentMovementDirection.y) > epsilon)
	{
		glm::vec3 vel{ glm::vec3{m_CurrentMovementDirection * m_Desc.movementSpeed, 0.f} };
		m_pRigidBody->SetVelociy(vel);
		m_CurrentMovementDirection = {};
		return;
	}
	m_pRigidBody->SetVelociy({});
}

void dae::CharacterController2D::SetCollider(BoxCollider2DComponent* pCollider)
{
	m_pCollider = pCollider;
	m_pRigidBody->AddCollider(pCollider);
}

void dae::CharacterController2D::Move(const glm::vec2& direction)
{
	m_CurrentMovementDirection = glm::normalize(direction);
}
