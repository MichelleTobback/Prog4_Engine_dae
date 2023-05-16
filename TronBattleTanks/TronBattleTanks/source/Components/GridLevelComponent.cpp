#include "GridLevelComponent.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"

dae::GridLevelComponent::GridLevelComponent(GameObject* pOwner, const GridLevelDesc& desc)
	: Component(pOwner)
	, m_Desc{ desc }
{
	m_pTiles.reserve(desc.Rows * desc.Cols);
	for (int y{}; y < desc.Rows; ++y)
	{
		for (int x{}; x < desc.Cols; ++x)
		{
			int i{ (y * desc.Cols) + x };
			const TileDesc& tileDesc{ desc.TileLayouts[desc.Tiles[i]] };
			glm::vec3 pos{x * tileDesc.Width, y * tileDesc.Height, 0.f};
			auto pTile{ GetOwner()->GetScene()->Instantiate(GetOwner(), pos) };
			m_pTiles.emplace_back(pTile->AddComponent<TileComponent>(this, tileDesc));
		}
	}
}
