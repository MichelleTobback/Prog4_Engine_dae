#pragma once
#include "Component.h"

namespace dae
{
	class GameObject;

	class RenderComponent : public Component
	{
	public:
		virtual ~RenderComponent() override = default;

		virtual void Render(const GameObject* pGameObject) const;

	private:

	};

	class TextureRenderComponent final : public RenderComponent
	{
	public:
		virtual ~TextureRenderComponent() override = default;

		virtual void Render(const GameObject* pGameObject) const override;

	private:
	};
}