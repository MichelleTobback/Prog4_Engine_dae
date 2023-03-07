#include "RenderComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include "TextureComponent.h"
#include "TransformComponent.h"

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

void dae::TextureRenderComponent::Render() const
{
	const glm::vec3& pos{ m_pTransformComponent->GetWorldPosition() };
	const glm::vec2& size{ m_pTextureComponent->GetSize() };
	const auto& pTexture{ m_pTextureComponent->GetTexture() };

	Renderer::GetInstance().RenderTexture(*pTexture, pos.x, pos.y, size.x, size.y);
}
