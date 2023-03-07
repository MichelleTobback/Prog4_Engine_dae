#pragma once
#include "Component.h"

namespace dae
{
	class GameObject;

	class RenderComponent : public Component
	{
	public:
		RenderComponent(GameObject* pOwner);
		virtual ~RenderComponent() override;

		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		virtual void Render() const;

	private:

	};

	class TextureComponent;
	class TransformComponent;
	class TextureRenderComponent final : public RenderComponent
	{
	public:
		TextureRenderComponent(GameObject* pOwner);
		virtual ~TextureRenderComponent() override = default;

		TextureRenderComponent(const TextureRenderComponent& other) = delete;
		TextureRenderComponent(TextureRenderComponent&& other) = delete;
		TextureRenderComponent& operator=(const TextureRenderComponent& other) = delete;
		TextureRenderComponent& operator=(TextureRenderComponent&& other) = delete;

		virtual void Render() const override;

	private:
		TextureComponent* m_pTextureComponent{ nullptr };
		TransformComponent* m_pTransformComponent{ nullptr };
	};
}