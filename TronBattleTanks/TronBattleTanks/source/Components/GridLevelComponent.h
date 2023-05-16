#pragma once

#include "Component/Component.h"
#include "Components/TileComponent.h"
#include "Component/SpriteAtlasComponent.h"

#include <glm/glm.hpp>
#include <vector>

namespace dae
{
	struct GridLevelDesc
	{
		int Rows{ 1 };
		int Cols{ 1 };
		SpriteAtlasComponent* pSpriteAtlas{ nullptr };
		std::vector<TileDesc> TileLayouts{};
		std::vector<size_t> Tiles{};
	};

	class GridLevelComponent final : public Component
	{
	public:
		GridLevelComponent(GameObject* pOwner, const GridLevelDesc& desc);
		virtual ~GridLevelComponent() override = default;

		GridLevelComponent(const GridLevelComponent& other) = delete;
		GridLevelComponent(GridLevelComponent&& other) = delete;
		GridLevelComponent& operator=(const GridLevelComponent& other) = delete;
		GridLevelComponent& operator=(GridLevelComponent&& other) = delete;

		inline const GridLevelDesc& GetDesc() const { return m_Desc; }

	private:
		GridLevelDesc m_Desc{};
		std::vector<TileComponent*> m_pTiles{};
	};
}