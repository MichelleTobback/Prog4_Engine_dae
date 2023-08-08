#pragma once
#include "Component.h"
#include "Component/QuadComponent.h"
#include "SpriteComponent.h"
#include "TextureComponent.h"
#include "Scene/Serializable.h"

namespace dae
{
	class GameObject;

	class RenderComponent : public Component
	{
	public:
		RenderComponent(GameObject* pOwner);
		virtual ~RenderComponent() override = default;

		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		virtual void Awake() override;
		virtual void Sleep() override;

		virtual void Render() const = 0;

		inline void SetLayer(size_t layer) { m_Layer = layer; }
		inline size_t GetLayer() const { return m_Layer; }

	private:
		size_t m_Layer{ 0 };
		bool m_Rendering{ false };
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

		virtual void Awake() override;
		virtual void Sleep() override;

		virtual void Render() const override;

		inline TextureComponent* GetTexture() const { return m_pTextureComponent; }
		inline void SetTexture(TextureComponent* pTextureComponent) { m_pTextureComponent = pTextureComponent; }

	private:
		TextureComponent* m_pTextureComponent{ nullptr };
		TransformComponent* m_pTransformComponent{ nullptr };
	};

	class SpriteComponent;
	class SpriteRenderComponent final : public RenderComponent
	{
	public:
		SpriteRenderComponent(GameObject* pOwner);
		SpriteRenderComponent(GameObject* pOwner, SpriteComponent* pSpriteComponent);
		virtual ~SpriteRenderComponent() override = default;

		SpriteRenderComponent(const TextureRenderComponent& other) = delete;
		SpriteRenderComponent(TextureRenderComponent&& other) = delete;
		SpriteRenderComponent& operator=(const TextureRenderComponent& other) = delete;
		SpriteRenderComponent& operator=(TextureRenderComponent&& other) = delete;

		virtual void Awake() override;
		virtual void Sleep() override;

		virtual void Render() const override;

		inline void SetSpriteComponent(SpriteComponent* pSpriteComponent) { m_pSpriteComponent = pSpriteComponent; }
		inline SpriteComponent* GetSprite() const { return m_pSpriteComponent; }

	private:
		SpriteComponent* m_pSpriteComponent;
		TransformComponent* m_pTransformComponent{ nullptr };
	};

	class QuadRendererComponent final : public RenderComponent
	{
	public:
		QuadRendererComponent(GameObject* pOwner);
		QuadRendererComponent(GameObject* pOwner, QuadComponent* pSpriteComponent);
		virtual ~QuadRendererComponent() override = default;

		QuadRendererComponent(const QuadRendererComponent& other) = delete;
		QuadRendererComponent(QuadRendererComponent&& other) = delete;
		QuadRendererComponent& operator=(const QuadRendererComponent& other) = delete;
		QuadRendererComponent& operator=(QuadRendererComponent&& other) = delete;

		virtual void Awake() override;
		virtual void Sleep() override;
		virtual void Render() const override;

		void SetQuad(QuadComponent* pQuad);

	private:
		QuadComponent* m_pQuad{nullptr};
		TransformComponent* m_pTransformComponent{ nullptr };
	};
}