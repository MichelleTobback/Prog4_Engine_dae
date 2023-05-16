#pragma once

#include "Component/Component.h"

#include <glm/glm.hpp>

namespace dae
{
	class GridLevelComponent;

	struct TileDesc
	{
		float Width{ 16.f };
		float Height{ 16.f };
		uint32_t m_SpriteIndex{};
	};

	class SpriteRenderComponent;
	class TileComponent final : public Component
	{
	public:
		TileComponent(GameObject* pOwner, GridLevelComponent* pGrid);
		TileComponent(GameObject* pOwner, GridLevelComponent* pGrid, const TileDesc& desc);
		virtual ~TileComponent() override = default;

		TileComponent(const TileComponent& other) = delete;
		TileComponent(TileComponent&& other) = delete;
		TileComponent& operator=(const TileComponent& other) = delete;
		TileComponent& operator=(TileComponent&& other) = delete;

		void SetWidth(float width);
		void SetHeight(float height);
		void SetSize(float width, float height);

		inline float GetWidth() const { return m_Desc.Width; }
		inline float GetHeight() const { return m_Desc.Height; }

	private:
		TileDesc m_Desc{};

		GridLevelComponent* m_pGrid{ nullptr };
		SpriteRenderComponent* m_pRenderer{ nullptr };
	};
}