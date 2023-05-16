#include "RenderComponent.h"

#include "Scene/GameObject.h"
#include "Renderer/Renderer.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "Renderer/Texture2D.h"

//=================================================
// RenderComponent
//=================================================

dae::RenderComponent::RenderComponent(GameObject* pOwner)
	: Component(pOwner)
{
	Renderer::GetInstance().m_pRenderComponents.push_back(this);
}

dae::RenderComponent::~RenderComponent()
{
	auto& renderComponents{ Renderer::GetInstance().m_pRenderComponents };
	renderComponents.erase(std::remove(renderComponents.begin(), renderComponents.end(), this), renderComponents.end());
}

void dae::RenderComponent::Render() const
{
	
}

//=================================================
// TextureRenderComponent
//=================================================

dae::TextureRenderComponent::TextureRenderComponent(GameObject* pOwner)
	: RenderComponent(pOwner)
{
	if (!pOwner->HasComponent<TextureComponent>())
		m_pTextureComponent = pOwner->AddComponent<TextureComponent>();
	else
		m_pTextureComponent = pOwner->GetComponent<TextureComponent>();

	m_pTransformComponent = pOwner->GetComponent<TransformComponent>();
}

dae::TextureRenderComponent::TextureRenderComponent(GameObject* pOwner, TextureComponent* pTextureComponent)
	: RenderComponent(pOwner)
	, m_pTextureComponent{pTextureComponent}
{
	m_pTransformComponent = pOwner->GetComponent<TransformComponent>();
}

void dae::TextureRenderComponent::Render() const
{
	glm::vec3 pos{ m_pTransformComponent->GetWorldPosition() };
	const glm::vec2& size{ m_pTextureComponent->GetSize() };
	pos.x -= size.x * 0.5f;
	pos.y -= size.y * 0.5f;
	const auto& pTexture{ m_pTextureComponent->GetTexture() };

	Renderer::GetInstance().RenderTexture(*pTexture, pos.x, pos.y, size.x, size.y, m_pTransformComponent->GetWorldRotation());
}

//=================================================
// SpriteRenderComponent
//=================================================

dae::SpriteRenderComponent::SpriteRenderComponent(GameObject* pOwner, SpriteComponent* pSpriteComponent)
	: RenderComponent(pOwner)
	, m_pSpriteComponent{pSpriteComponent}
{
	m_pTransformComponent = pOwner->GetComponent<TransformComponent>();
}

void dae::SpriteRenderComponent::Render() const
{
	glm::vec3 pos{ m_pTransformComponent->GetWorldPosition() };
	//const glm::vec2& size{ m_pSpriteComponent->GetTexture().GetSize()};
	//pos.x -= size.x * 0.5f;
	//pos.y -= size.y * 0.5f;
	const auto& pTexture{ m_pSpriteComponent->GetTexture().GetTexture() };
	auto& src{ m_pSpriteComponent->GetSource() };

	Renderer::GetInstance().RenderTexture(*pTexture, pos.x, pos.y, src.x, src.y, src.z, src.w, m_pTransformComponent->GetWorldRotation());
}
