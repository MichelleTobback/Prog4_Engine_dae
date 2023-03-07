#include "RotationComponent.h"

#include "TransformComponent.h"
#include "GameObject.h"
#include "Time.h"

#include <glm/gtx/rotate_vector.hpp>

dae::RotateComponent::RotateComponent(
	GameObject* pGameObject, float rotationSpeed)
	: m_RotationSpeed{ rotationSpeed }, Component(pGameObject)
{
	m_pTransformComponent = GetOwner()->GetComponent<TransformComponent>();
}

void dae::RotateComponent::Update()
{
	glm::vec3 currentPos{ m_pTransformComponent->GetLocalPosition() };
	float deltaTime{ Time::GetInstance().GetDeltaTime() };
	m_pTransformComponent->SetLocalPosition(glm::rotateZ(currentPos, m_RotationSpeed * deltaTime));
}
