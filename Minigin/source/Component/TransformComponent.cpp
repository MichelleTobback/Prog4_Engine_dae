#include "TransformComponent.h"
#include "Scene/GameObject.h"

dae::TransformComponent::TransformComponent(GameObject* pOwner)
	: Component(pOwner)
{
}

const glm::vec3& dae::TransformComponent::GetWorldPosition()
{
	if (IsDirty(TransformFlag::Position))
		RecalculateWorldPosition();

	return m_WorldPosition;
}

float dae::TransformComponent::GetWorldRotation()
{
	if (IsDirty(TransformFlag::Rotation))
		RecalculateWorldRotation();

	return m_WorldRotation;
}

void dae::TransformComponent::SetLocalPosition(const glm::vec3 position)
{
	m_LocalPosition = position;
	SetDirty(TransformFlag::Position, true);
}

void dae::TransformComponent::SetLocalRotation(float rotation)
{
	m_LocalRotation = rotation;
	SetDirty(TransformFlag::Rotation, true);
	SetDirty(TransformFlag::Forward, false);
}

void dae::TransformComponent::Translate(const glm::vec3 translation)
{
	m_LocalPosition += translation;
	SetDirty(TransformFlag::Position, true);
}

void dae::TransformComponent::Rotate(float rotation)
{
	m_LocalRotation += rotation;
	SetDirty(TransformFlag::Rotation, true);

	for (auto pChild : GetOwner()->GetChildren())
	{
		pChild->GetTransform().SetDirty(TransformFlag::Position, true);
	}
}

const glm::vec2& dae::TransformComponent::GetForward()
{
	if (IsDirty(TransformFlag::Forward))
		RecalculateForward();
	return m_Forward;
}

void dae::TransformComponent::RecalculateWorldPosition()
{
	auto pParent{ GetOwner()->GetParent() };

	if (pParent == nullptr)
		m_WorldPosition = m_LocalPosition;
	else
	{
		auto pTransformComponent{ &pParent->GetTransform() };
		const float rotation{ glm::radians(pTransformComponent->GetLocalRotation()) };
		if (rotation > 0.f)
		{
			m_WorldPosition = pTransformComponent->GetWorldPosition() + m_LocalPosition * glm::vec3{ std::cosf(rotation), std::sinf(rotation), 0.f };
		}
		else
		{
			m_WorldPosition = pTransformComponent->GetWorldPosition() + m_LocalPosition;
		}
	}
	SetDirty(TransformFlag::Position, false);
}

void dae::TransformComponent::RecalculateWorldRotation()
{
	auto pParent{ GetOwner()->GetParent() };

	if (pParent == nullptr)
		m_WorldRotation = m_LocalRotation;
	else
	{
		auto pTransformComponent{ &pParent->GetTransform() };
		m_WorldRotation = pTransformComponent->GetWorldRotation() + m_LocalRotation;
	}
	SetDirty(TransformFlag::Rotation, false);
	SetDirty(TransformFlag::Forward, true);
}

void dae::TransformComponent::SetDirty(TransformFlag flag, bool isDirty)
{
	if (isDirty)
	{
		m_DirtyFlags |= static_cast<int>(flag);

		auto& children{ GetOwner()->GetChildren() };
		for (auto& child : children)
		{
			child->GetTransform().SetDirty(flag, isDirty);
		}
	}
	else
		m_DirtyFlags &= ~static_cast<int>(flag);
}

bool dae::TransformComponent::IsDirty(TransformFlag flag) const
{
	return m_DirtyFlags & static_cast<int>(flag);
}

void dae::TransformComponent::RecalculateForward()
{
	float worldRot{ glm::radians(GetWorldRotation()) };
	m_Forward = { std::cosf(worldRot), std::sinf(worldRot) };
	SetDirty(TransformFlag::Forward, false);
}
