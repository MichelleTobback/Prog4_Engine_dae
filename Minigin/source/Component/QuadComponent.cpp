#include "QuadComponent.h"
#include "Scene/GameObject.h"

dae::QuadComponent::QuadComponent(GameObject* pOwner, const glm::vec2& size, const glm::vec4& color, bool isSolid)
    : Component(pOwner)
    , m_Size{size}
    , m_Color{color}
    , m_IsSolid{isSolid}
{
    m_Vertices.resize(m_NumVerts);
}

const glm::vec2& dae::QuadComponent::GetSize() const
{
    return m_Size;
}

void dae::QuadComponent::SetSize(const glm::vec2 size)
{
    m_Size = size;
    m_VerticesDirty = true;
}

const glm::vec4& dae::QuadComponent::GetColor() const
{
    return m_Color;
}

void dae::QuadComponent::SetColor(const glm::vec4& color)
{
    m_Color = color;
}

const std::vector<glm::vec2>& dae::QuadComponent::GetVertices()
{
    if (m_VerticesDirty)
        RecalculateVertices();

    return m_Vertices;
}

void dae::QuadComponent::GetVerticesWorldPosition(std::vector<glm::vec2>& vertsOut)
{
    if (vertsOut.size() < m_NumVerts)
        vertsOut.resize(m_NumVerts);

    const glm::vec2 pos{ GetTransform().GetWorldPosition() };
    const auto& verts{ GetVertices() };
    for (size_t i{}; i < m_NumVerts; ++i)
    {
        vertsOut[i] = pos + verts[i];
    }
}

void dae::QuadComponent::RecalculateVertices()
{
    m_Vertices[0] = { 0.f, 0.f };
    m_Vertices[1] = { m_Size.x, 0.f };
    m_Vertices[2] = { m_Size.x, m_Size.y };
    m_Vertices[3] = { 0.f, m_Size.y };

    m_VerticesDirty = false;
}
