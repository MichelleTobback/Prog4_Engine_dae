#include "RenderComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include "TextureComponent.h"
#include "TransformComponent.h"

void dae::RenderComponent::Render(const GameObject*) const
{
	
}

void dae::TextureRenderComponent::Render(const GameObject* pGameObject) const
{
	auto& textureComponent{ pGameObject->GetComponent<TextureComponent>() };
	auto& transformComponent{ pGameObject->GetComponent<TransformComponent>() };
	const glm::vec3& pos{ transformComponent.GetPosition() };
	const glm::vec2& size{ textureComponent.GetSize() };

	Renderer::GetInstance().RenderTexture(*textureComponent.GetTexture().get(), pos.x, pos.y, size.x, size.y);
}
