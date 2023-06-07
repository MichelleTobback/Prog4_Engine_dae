#include "QuadComponent.h"
#include "Scene/GameObject.h"

dae::QuadComponent::QuadComponent(GameObject* pOwner, const glm::vec2& size, const glm::vec4& color, bool isSolid)
    : Component(pOwner)
    , m_Size{size}
    , m_Color{color}
    , m_IsSolid{isSolid}
{
}

const glm::vec2& dae::QuadComponent::GetSize() const
{
    return m_Size;
}

void dae::QuadComponent::SetSize(const glm::vec2 size)
{
    m_Size = size;
}

const glm::vec4& dae::QuadComponent::GetColor() const
{
    return m_Color;
}

void dae::QuadComponent::SetColor(const glm::vec4& color)
{
    m_Color = color;
}
