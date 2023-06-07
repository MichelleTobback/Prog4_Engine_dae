#include "RenderComponent.h"

#include "Scene/GameObject.h"
#include "Renderer/Renderer.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "Renderer/Texture2D.h"
#include "Component/QuadComponent.h"

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
	const auto& pTexture{ m_pTextureComponent->GetTexture() };

	Renderer::GetInstance().RenderTexture(*pTexture, pos.x, pos.y, size.x, size.y, m_pTransformComponent->GetWorldRotation());
}

//=================================================
// SpriteRenderComponent
//=================================================

dae::SpriteRenderComponent::SpriteRenderComponent(GameObject* pOwner)
	: SpriteRenderComponent(pOwner, nullptr)
{
}

dae::SpriteRenderComponent::SpriteRenderComponent(GameObject* pOwner, SpriteComponent* pSpriteComponent)
	: RenderComponent(pOwner)
	, m_pSpriteComponent{pSpriteComponent}
{
	
}

void dae::SpriteRenderComponent::Awake()
{
	m_pTransformComponent = &GetOwner()->GetTransform();
}

void dae::SpriteRenderComponent::Render() const
{
	glm::vec3 pos{ m_pTransformComponent->GetWorldPosition() };
	auto pTexture{ (m_pSpriteComponent && m_pSpriteComponent->GetTexture()) ? m_pSpriteComponent->GetTexture()->GetTexture() : nullptr };
	if (pTexture)
	{
		auto& src{ m_pSpriteComponent->GetSource() };

		Renderer::GetInstance().RenderTexture(*pTexture, pos.x, pos.y, src.x, src.y, src.z, src.w, m_pTransformComponent->GetWorldRotation());
	}
}

//=================================================
// QuadRendererComponent
//=================================================

void dae::QuadRendererComponent::Awake()
{
	m_pTransformComponent = &GetOwner()->GetTransform();
}

dae::QuadRendererComponent::QuadRendererComponent(GameObject* pOwner)
	: QuadRendererComponent(pOwner, nullptr)
{

}

dae::QuadRendererComponent::QuadRendererComponent(GameObject* pOwner, QuadComponent* pQuadComponent)
	: RenderComponent(pOwner)
	, m_pQuad{ pQuadComponent }
{
	
}

void dae::QuadRendererComponent::Render() const
{
	glm::vec3 pos{ m_pTransformComponent->GetWorldPosition() };
	const glm::vec2& size{ m_pQuad->GetSize() };

	if (m_pQuad->IsSolid())
		Renderer::GetInstance().RenderSolidQuad(pos.x, pos.y, size.x, size.y, m_pQuad->GetColor());
	else
		Renderer::GetInstance().RenderQuad(pos.x, pos.y, size.x, size.y, m_pQuad->GetColor());
}

void dae::QuadRendererComponent::SetQuad(dae::QuadComponent* pQuad)
{
	m_pQuad = pQuad;
}
