#include "TransformComponent.h"
#include "Scene/GameObject.h"
#include "Core/Math.h"

dae::TransformComponent::TransformComponent(GameObject* pOwner)
	: Component(pOwner)
    , m_LocalRotQ{ glm::quat(1, 0, 0, 0) }
    , m_WorldRotQ{ glm::quat(1, 0, 0, 0) }
{
}

const glm::vec3& dae::TransformComponent::GetWorldPosition()
{
	if (IsDirty(TransformFlag::Position))
	{
		RecalculateWorldPosition();
	}
	return m_WorldPosition;
}

const glm::quat& dae::TransformComponent::GetWorldRotation()
{
	if (IsDirty(TransformFlag::Rotation))
	{
		RecalculateWorldRotation();
	}
	return m_WorldRotQ;
}

float dae::TransformComponent::GetWorldRotationAngle()
{
	glm::vec3 eulerAngles{ glm::degrees(glm::eulerAngles(GetWorldRotation())) };
	float angle{ eulerAngles.z };

	if (angle < 0.0f)
	{
		angle += 360.0f;
	}

	return angle;
}

void dae::TransformComponent::SetLocalPosition(const glm::vec3& position)
{
	m_LocalPosition = position;
	SetDirty(TransformFlag::Position, true);
}

void dae::TransformComponent::SetLocalRotation(float rotation)
{
	m_LocalRotation = rotation;
	m_LocalRotQ = glm::angleAxis(glm::radians(rotation), glm::vec3(0, 0, 1));
	SetDirty(TransformFlag::Rotation, true);
	SetDirty(TransformFlag::Position, true);
	SetDirty(TransformFlag::Forward, true);
}

void dae::TransformComponent::Translate(const glm::vec3& translation)
{
	m_LocalPosition += translation;
	SetDirty(TransformFlag::Position, true);
}

void dae::TransformComponent::Rotate(float rotation)
{
	m_LocalRotation += rotation;
	m_LocalRotation = Math::WrapAngle(m_LocalRotation);

	m_LocalRotQ = glm::angleAxis(glm::radians(m_LocalRotation), glm::vec3(0, 0, 1));
	SetDirty(TransformFlag::Rotation, true);
	SetDirty(TransformFlag::Position, true);
	SetDirty(TransformFlag::Forward, true);
}

const glm::vec3& dae::TransformComponent::GetForward()
{
	if (IsDirty(TransformFlag::Forward))
		RecalculateForward();
	return m_Forward;
}

void dae::TransformComponent::RecalculateWorldPosition()
{
	if (GetOwner()->GetParent() != nullptr)
	{
		const glm::vec3& parentPosition{ GetOwner()->GetParent()->GetTransform().GetWorldPosition() };
		const glm::quat& worldRotation{ GetOwner()->GetParent()->GetTransform().GetWorldRotation() };
		m_WorldPosition = parentPosition + (worldRotation * m_LocalPosition);
	}
	else
	{
		m_WorldPosition = m_LocalPosition;
	}
	SetDirty(TransformFlag::Position, false);
}

void dae::TransformComponent::RecalculateWorldRotation()
{
	if (GetOwner()->GetParent() != nullptr)
	{
		const glm::quat& parentRotation{ GetOwner()->GetParent()->GetTransform().GetWorldRotation() };
		m_WorldRotQ = m_LocalRotQ * parentRotation;
	}
	else
	{
		m_WorldRotQ = m_LocalRotQ;
	}
	SetDirty(TransformFlag::Rotation, false);
}

void dae::TransformComponent::SetDirty(TransformFlag flag, bool isDirty)
{
	if (isDirty)
	{
		m_DirtyFlags |= static_cast<int>(flag);
		SetChildrenDirty(flag);
	}
	else
		m_DirtyFlags &= ~static_cast<int>(flag);
}

bool dae::TransformComponent::IsDirty(TransformFlag flag) const
{
	return (m_DirtyFlags & static_cast<int>(flag)) != 0;
}

void dae::TransformComponent::SetChildrenDirty(TransformFlag flag)
{
	for (auto child : GetOwner()->GetChildren())
	{
		child->GetTransform().SetDirty(flag, true);
	}
}

void dae::TransformComponent::RecalculateForward()
{
	m_Forward = glm::rotate(GetWorldRotation(), glm::vec3(1.f, 0.f, 0.f));
	SetDirty(TransformFlag::Forward, false);
}
