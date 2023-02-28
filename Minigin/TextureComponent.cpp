#include "TextureComponent.h"
#include "ResourceManager.h"

#include "Texture2D.h"

const std::shared_ptr<dae::Texture2D>& dae::TextureComponent::GetTexture() const
{
    return m_pTexture;
}

void dae::TextureComponent::SetTexture(const std::string& path)
{
    m_pTexture = ResourceManager::GetInstance().LoadTexture(path);

    auto size{ m_pTexture->GetSize() };

    m_Size = { size.x, size.y };
}

void dae::TextureComponent::SetTexture(const std::shared_ptr<Texture2D>& pTexture)
{
    m_pTexture = pTexture;

    auto size{ m_pTexture->GetSize() };

    m_Size = { size.x, size.y };
}

const glm::vec2& dae::TextureComponent::GetSize() const
{
    return m_Size;
}

void dae::TextureComponent::SetSize(const glm::vec2 size)
{
    m_Size = size;
}
