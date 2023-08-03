#pragma once
#include "Component/Component.h"

#include "Core/Delegate.h"

#include <glm/glm.hpp>
#include <functional>

namespace dae
{
	class ObjectSpawner;
	class NodeComponent;
	class BurgerPlate;
	class GameObject;
	struct BTTile
	{
		BTTile(const glm::vec3& _pos, uint32_t _index)
			:pInstance{ nullptr }, pPlate{nullptr}, pNode{nullptr}, pSpawner{ nullptr }, pos{_pos}, index{_index}
		{}

		GameObject* pInstance;
		BurgerPlate* pPlate;
		NodeComponent* pNode;
		ObjectSpawner* pSpawner;
		glm::vec3 pos;
		uint32_t index{};

		operator bool() const
		{
			return pInstance;
		}
	};

	using BTTileFunc = std::function<void(BTTile&)>;
	class BTTileGridComponent final : public Component
	{
	public:
		BTTileGridComponent(GameObject* pOwner, uint32_t x, uint32_t y, float tileSize);
		virtual ~BTTileGridComponent() = default;

		BTTileGridComponent(const BTTileGridComponent& other) = delete;
		BTTileGridComponent(BTTileGridComponent&& other) = delete;
		BTTileGridComponent& operator=(const BTTileGridComponent& other) = delete;
		BTTileGridComponent& operator=(BTTileGridComponent&& other) = delete;

		void Awake() override;

		BTTile& GetClosestTile(const glm::vec3& worldPos);
		BTTile& GetTile(const glm::vec3& worldPos);
		BTTile& GetTile(const glm::u32vec2& pos);
		BTTile& GetTile(uint32_t index);

		void ForEachTile(const BTTileFunc& fn);
		void ForEachSpawner(const BTTileFunc& fn);

		bool IsValidIndex(uint32_t index);
		bool IsValidTile(const glm::u32vec2& gridPos);
		bool IsValidTile(const glm::vec3& worldPos);
		glm::u32vec2 GetPos(uint32_t index) const;
		uint32_t GetIndex(const glm::u32vec2& gridPos) const;
		uint32_t GetIndex(const glm::vec3& worldPos) const;

		const glm::u32vec2& GetSize() const;
		uint32_t GetTileCount() const;
		float GetTileSize() const;

		Delegate<void()>& GetOnGridMappedDelegate() { return m_OnGridMappedDelegate; }

	private:
		void MapGrid();

		Delegate<void()> m_OnGridMappedDelegate{};
		std::vector<std::unique_ptr<BTTile>> m_pGrid;
		std::vector<uint32_t> m_Spawners;
		glm::u32vec2 m_GridSize;
		uint32_t m_NumTiles;
		float m_TileSize;
	};
}