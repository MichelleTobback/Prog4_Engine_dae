#include "CircleComponent.h"

dae::CircleComponent::CircleComponent(GameObject* pOwner, float radius, const glm::vec4& color, bool isSolid)
    : Component(pOwner)
    , m_Radius{radius}
    , m_Color{ color }
    , m_IsSolid{ isSolid }
{
}

float dae::CircleComponent::GetRadius() const
{
    return m_Radius;
}

void dae::CircleComponent::SetRadius(float radius)
{
    m_Radius = radius;
}

const glm::vec4& dae::CircleComponent::GetColor() const
{
    return m_Color;
}

void dae::CircleComponent::SetColor(const glm::vec4& color)
{
    m_Color = color;
}
