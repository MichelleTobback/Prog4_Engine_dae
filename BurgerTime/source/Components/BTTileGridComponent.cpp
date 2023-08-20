#include "BTTileGridComponent.h"
#include "Scene/Scene.h"

#include "Scene/GameObject.h"
#include "Components/BurgerPlate.h"
#include "Components/NodeComponent.h"
#include "Components/ObjectSpawner.h"

dae::BTTileGridComponent::BTTileGridComponent(GameObject* pOwner, uint32_t cols, uint32_t rows, float tileSize)
	: Component(pOwner), m_GridSize { cols, rows }, m_NumTiles{ cols * rows }, m_TileSize{ tileSize }
{
	uint32_t index{};
	m_pGrid.resize(m_NumTiles);
	for (auto& pTile : m_pGrid)
	{
		const glm::u32vec2 pos{ GetPos(index) };
		const glm::vec3 worldPos{ pos.x * tileSize, pos.y * tileSize, 0.f};
		pTile = std::make_unique<BTTile>(worldPos, index);
		++index;
	}
	MapGrid();
}

void dae::BTTileGridComponent::Awake()
{
	MapGrid();
}

dae::BTTile& dae::BTTileGridComponent::GetClosestTile(const glm::vec3& worldPos)
{
	BTTile& closest{ *m_pGrid[0] };
	float closestDistance{ FLT_MAX };

	ForEachTile([&](BTTile& tile)
		{
			float distance{ glm::distance2(closest.pos, worldPos) };

			if (distance < closestDistance)
			{
				closestDistance = distance;
				closest = tile;
			}
		});

	return closest;
}

dae::BTTile& dae::BTTileGridComponent::GetTile(const glm::vec3& worldPos)
{
	const uint32_t index{ GetIndex(worldPos) };
	return GetTile(index);
}

dae::BTTile& dae::BTTileGridComponent::GetTile(const glm::u32vec2& pos)
{
	const uint32_t index{ GetIndex(pos) };
	return GetTile(index);
}

bool dae::BTTileGridComponent::IsValidIndex(uint32_t index)
{
	return m_NumTiles > 0 && index < m_NumTiles;
}

bool dae::BTTileGridComponent::IsValidTile(const glm::u32vec2& gridPos)
{
	return IsValidIndex(GetIndex(gridPos));
}

bool dae::BTTileGridComponent::IsValidTile(const glm::vec3& worldPos)
{
	return IsValidIndex(GetIndex(worldPos));
}

glm::u32vec2 dae::BTTileGridComponent::GetPos(uint32_t index) const
{
	return
	{
		index % m_GridSize.x,
		index / m_GridSize.x
	};
}

dae::BTTile& dae::BTTileGridComponent::GetTile(uint32_t index)
{
	return *m_pGrid[index];
}

void dae::BTTileGridComponent::ForEachTile(const BTTileFunc& fn)
{
	std::for_each(m_pGrid.begin(), m_pGrid.end(), [this, fn](auto& pTile)
		{
			fn(*pTile);
		});
}

void dae::BTTileGridComponent::ForEachSpawner(const BTTileFunc& fn)
{
	std::for_each(m_Spawners.begin(), m_Spawners.end(), [this, fn](uint32_t index)
		{
			fn(*(m_pGrid[index]));
		});
}

void dae::BTTileGridComponent::MapGrid()
{
	m_Spawners.clear();
	const auto& gridObjects{ GetOwner()->GetChildren() };
	for (size_t i{}; i < gridObjects.size(); ++i)
	{
		GameObject* pTile{ gridObjects[i] };
		const glm::vec3& worldPos{ pTile->GetTransform().GetWorldPosition() };
		const uint32_t index{ GetIndex(worldPos) };

		if (!IsValidIndex(index))
			continue;

		BTTile& tile{ GetTile(index) };
		tile.pInstance = pTile;
		tile.pos = worldPos;

		if (tile.pInstance->HasComponent<BurgerPlate>())
			tile.pPlate = tile.pInstance->GetComponent<BurgerPlate>();
		if (tile.pInstance->HasComponent<NodeComponent>())
			tile.pNode = tile.pInstance->GetComponent<NodeComponent>();
		if (tile.pInstance->HasComponent<ObjectSpawner>())
		{
			tile.pSpawner = tile.pInstance->GetComponent<ObjectSpawner>();
			m_Spawners.push_back(index);
		}
	}

	m_OnGridMappedDelegate.Invoke();
}

uint32_t dae::BTTileGridComponent::GetIndex(const glm::u32vec2& gridPos) const
{
	return gridPos.y * m_GridSize.x + gridPos.x;
}

uint32_t dae::BTTileGridComponent::GetIndex(const glm::vec3& worldPos) const
{
	return static_cast<uint32_t>(worldPos.y / m_TileSize) * m_GridSize.x + static_cast<uint32_t>(worldPos.x / m_TileSize);
}

const glm::u32vec2& dae::BTTileGridComponent::GetSize() const
{
	return m_GridSize;
}

uint32_t dae::BTTileGridComponent::GetTileCount() const
{
	return static_cast<uint32_t>(m_pGrid.size());
}

float dae::BTTileGridComponent::GetTileSize() const
{
	return m_TileSize;
}
