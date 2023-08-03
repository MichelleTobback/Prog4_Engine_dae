#include "BTGameMode.h"
#include "Managers/SceneManager.h"
#include "Scene/Scene.h"
#include "Components/ObjectSpawner.h"
#include "Components/HealthComponent.h"

#include "BurgerTime.h"

void dae::BTGameMode::OnSceneLoaded()
{
	CreateGrid();

	m_pGrid->GetOnGridMappedDelegate() += [this]()
	{
		SpawnPlayer();
		m_pHUD = CreateHUD();
	};
}

void dae::BTGameMode::CreateGrid()
{
	auto pObjects{ SceneManager::GetInstance().GetCurrent()->GetGameObjectWithTag("Lvl")};
	m_pLevelRoot = pObjects.size() > 0 ? pObjects[0] : nullptr;

	if (!m_pLevelRoot)
		return;

	if (m_pLevelRoot->HasComponent<BTTileGridComponent>())
		m_pGrid = m_pLevelRoot->GetComponent<BTTileGridComponent>();
	else
	{
		const glm::vec2 size{ 208.f, 200.f };
		const float tileSize{ 8.f };
		glm::u32vec2 gridSize{ static_cast<glm::u32vec2>(size / tileSize) };
		gridSize.x += 2;
		++gridSize.y;
		m_pGrid = m_pLevelRoot->AddComponent<BTTileGridComponent>(gridSize.x, gridSize.y, tileSize);
	}
}

void dae::BTGameMode::RespawnAllActiveObjects()
{
	m_pGrid->ForEachSpawner([](BTTile& tile)
		{
			if (tile.pSpawner->GetInstance() && tile.pSpawner->GetInstance()->IsActive())
				tile.pSpawner->Spawn();
		});
}

void dae::BTGameMode::RespawnAll()
{
	m_pGrid->ForEachSpawner([](BTTile& tile)
		{
			tile.pSpawner->Spawn();
			HealthComponent* pHealth{ tile.pSpawner->GetInstance()->GetComponent<HealthComponent>() };
			if (pHealth)
				pHealth->Heal();
		});
}

void dae::BTGameMode::SpawnPlayer()
{
	m_pGrid->ForEachSpawner([](BTTile& tile)
		{
			if (tile.pSpawner->GetSpawnID() == static_cast<uint32_t>(BurgerTime::SpawnID::Player))
				tile.pSpawner->Spawn();
		});
}
