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
		TextureRenderComponent(GameObject* pOwner, TextureComponent* pTextureComponent);
		virtual ~TextureRenderComponent() override = default;

		TextureRenderComponent(const TextureRenderComponent& other) = delete;
		TextureRenderComponent(TextureRenderComponent&& other) = delete;
		TextureRenderComponent& operator=(const TextureRenderComponent& other) = delete;
		TextureRenderComponent& operator=(TextureRenderComponent&& other) = delete;

		virtual void Render() const override;

		inline TextureComponent* GetTexture() const { return m_pTextureComponent; }

	private:
		TextureComponent* m_pTextureComponent{ nullptr };
		TransformComponent* m_pTransformComponent{ nullptr };
	};

	class SpriteComponent;
	class SpriteRenderComponent final : public RenderComponent
	{
	public:
		SpriteRenderComponent(GameObject* pOwner, SpriteComponent* pSpriteComponent);
		virtual ~SpriteRenderComponent() override = default;

		SpriteRenderComponent(const TextureRenderComponent& other) = delete;
		SpriteRenderComponent(TextureRenderComponent&& other) = delete;
		SpriteRenderComponent& operator=(const TextureRenderComponent& other) = delete;
		SpriteRenderComponent& operator=(TextureRenderComponent&& other) = delete;

		virtual void Render() const override;

		inline SpriteComponent* GetSprite() const { return m_pSpriteComponent; }

	private:
		SpriteComponent* m_pSpriteComponent;
		TransformComponent* m_pTransformComponent{ nullptr };
	};
}