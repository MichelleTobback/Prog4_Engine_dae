#include "TransformComponent.h"
#include "GameObject.h"

dae::TransformComponent::TransformComponent(GameObject* pOwner)
	: Component(pOwner)
{
}

const glm::vec3& dae::TransformComponent::GetWorldPosition()
{
	if (m_IsPositionDirty)
		RecalculateWorldPosition();

	return m_WorldPosition;
}

void dae::TransformComponent::SetLocalPosition(const glm::vec3 position)
{
	m_LocalPosition = position;
	m_IsPositionDirty = true;
}

void dae::TransformComponent::RecalculateWorldPosition()
{
	auto pParent{ GetOwner()->GetParent() };

	if (pParent == nullptr)
		m_WorldPosition = m_LocalPosition;
	else
	{
		auto pTransformComponent{ pParent->GetComponent<TransformComponent>() };
		m_WorldPosition = pTransformComponent->GetWorldPosition() + m_LocalPosition;
	}
	m_IsPositionDirty = false;

}
