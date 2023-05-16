#include "SpriteAtlasComponent.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"

dae::SpriteAtlasComponent::SpriteAtlasComponent(GameObject* pOwner, TextureComponent* pTexture)
	: Component(pOwner)
	, m_pTexture{pTexture}
{
}

void dae::SpriteAtlasComponent::SetTexture(TextureComponent* pTexture)
{
	m_pTexture = pTexture;

	for (auto pSprite : m_pSprites)
	{
		pSprite->SetTexture(pTexture, pSprite->GetSource());
	}
}

uint32_t dae::SpriteAtlasComponent::AddSprite(const glm::vec4& src)
{
	auto pSprite{ GetOwner()->GetScene()->Instantiate(GetOwner()) };
	auto pSpriteComponent{ pSprite->AddComponent<SpriteComponent>(m_pTexture, src) };
	
	return AddSprite(pSpriteComponent);
}

uint32_t dae::SpriteAtlasComponent::AddSprite(float srcX, float srcY, float srcWidth, float srcHeight)
{
	return AddSprite({srcX, srcY, srcWidth, srcHeight});
}

uint32_t dae::SpriteAtlasComponent::AddSprite(SpriteComponent* pSprite)
{
	uint32_t index{ static_cast<uint32_t>(m_pSprites.size()) };
	m_pSprites.push_back(pSprite);
	return index;
}
