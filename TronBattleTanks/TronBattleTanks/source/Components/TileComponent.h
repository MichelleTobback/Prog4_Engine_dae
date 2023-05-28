#pragma once

#include "Component/Component.h"
#include "Component/RenderComponent.h"
#include "Scene/Serializable.h"

#include <glm/glm.hpp>

namespace dae
{
	class GridLevelComponent;
	enum class TileFlag
	{
		None = 0,
		Sprite = 1, Quad = 2
	};

	struct TileDesc
	{
		QuadRendererComponent* pQuadRenderer{ nullptr };
		SpriteRenderComponent* pRenderer{ nullptr };
		TileFlag flags{ TileFlag::Quad };
	};

	class SpriteRenderComponent;
	class TileComponent final : public Component, public Serializable
	{
	public:
		TileComponent(GameObject* pOwner);
		TileComponent(GameObject* pOnwer, const TileDesc& desc);
		virtual ~TileComponent() override = default;

		TileComponent(const TileComponent& other) = delete;
		TileComponent(TileComponent&& other) = delete;
		TileComponent& operator=(const TileComponent& other) = delete;
		TileComponent& operator=(TileComponent&& other) = delete;

		virtual void Serialize(BinaryWriter& out) const override;
		static bool Deserialize(DeserializeParams& params);

		TileDesc& GetTileDesc();

	protected:

	private:
		TileDesc m_Desc{};
	};
}