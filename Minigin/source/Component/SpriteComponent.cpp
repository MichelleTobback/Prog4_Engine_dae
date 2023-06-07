#include "SpriteComponent.h"

dae::SpriteComponent::SpriteComponent(GameObject* pOwner, TextureComponent* pTexture, const glm::vec4& src)
	: Component(pOwner)
	, m_pTexture{pTexture}
	, m_Source{src}
{
	SetSource(src);
}

dae::SpriteComponent::SpriteComponent(GameObject* pOwner, TextureComponent* pTexture, float srcX, float srcY, float srcWidth, float srcHeight)
	: SpriteComponent::SpriteComponent(pOwner, pTexture, {srcX, srcY, srcWidth, srcHeight})
{
}

void dae::SpriteComponent::SetTexture(TextureComponent* pTexture, bool texSizeIsSrc)
{
	m_pTexture = pTexture;
	if (texSizeIsSrc)
		SetSource({0.f, 0.f, 0.f, 0.f});
}

void dae::SpriteComponent::SetTexture(TextureComponent* pTexture, const glm::vec4& src)
{
	m_pTexture = pTexture;

	SetSource(src);
}

void dae::SpriteComponent::SetTexture(TextureComponent* pTexture, float srcX, float srcY, float srcWidth, float srcHeight)
{
	SetTexture(pTexture, { srcX, srcY, srcWidth, srcHeight });
}

void dae::SpriteComponent::SetSource(const glm::vec4& src)
{
	m_Source = src;

	if (m_pTexture != nullptr)
	{
		if (src.z <= 0.f)
			m_Source.z = m_pTexture->GetSize().x;
		if (src.w <= 0.f)
			m_Source.w = m_pTexture->GetSize().y;
	}
}

void dae::SpriteComponent::SetSource(float srcX, float srcY, float srcWidth, float srcHeight)
{
	SetSource({ srcX, srcY, srcWidth, srcHeight });
}
