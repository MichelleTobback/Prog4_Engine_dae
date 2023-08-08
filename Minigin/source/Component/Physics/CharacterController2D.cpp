#include "CharacterController2D.h"

dae::CharacterController2D::CharacterController2D(GameObject* pOwner, const CharacterController2DDesc& desc)
	: Component(pOwner), m_Desc{desc}
{
}

void dae::CharacterController2D::Update()
{
	if (m_Move)
	{
		glm::vec3 vel{ glm::vec3{m_CurrentMovementDirection * m_Desc.movementSpeed, 0.f} };
		m_pRigidBody->SetVelociy(vel);
		m_Move = false;
		return;
	}
	m_pRigidBody->SetVelociy({});
}

void dae::CharacterController2D::LateUpdate()
{
	
}

void dae::CharacterController2D::SetCollider(BoxCollider2DComponent* pCollider)
{
	m_pCollider = pCollider;
	m_pRigidBody->AddCollider(pCollider);
}

void dae::CharacterController2D::Move(const glm::vec2& direction)
{
	const float epsilon{ 0.1f };
	if (glm::length(direction) > epsilon)
	{
		m_CurrentMovementDirection = glm::normalize(direction);
		m_Move = true;
	}
}
