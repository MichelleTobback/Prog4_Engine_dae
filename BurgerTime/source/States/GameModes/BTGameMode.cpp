#include "BTGameMode.h"
#include "Managers/SceneManager.h"
#include "Scene/Scene.h"
#include "Components/ObjectSpawner.h"
#include "Components/HealthComponent.h"
#include "Components/PlayerController.h"
#include "Components/CharacterInfo.h"
#include "BurgerTime.h"
#include "States/GameModes/MainMenuGameMode.h"

dae::ObservableType<uint32_t> dae::BTGameMode::m_Score{};

dae::BTGameMode::BTGameMode(size_t level)
 : m_CurrentLevel{level}
{
	LoadLevels();
}

void dae::BTGameMode::OnEnter()
{
	m_Ingredients = 0;
	if (m_CurrentLevel == 0)
		m_Score = 0;

	SceneManager::GetInstance().GetOnSceneLoaded() += std::bind(&BTGameMode::OnSceneLoaded, this, std::placeholders::_1, std::placeholders::_2);
	SceneManager::GetInstance().OpenSceneByIndex(m_ScenesIndices[m_CurrentLevel]);
}

void dae::BTGameMode::OnExit()
{
	if (m_pGrid)
	{
		RespawnAll();
		m_pGrid->GetOnGridMappedDelegate().Clear();
	}
	m_pGrid = nullptr;
	SceneManager::GetInstance().GetCurrent()->Sleep();
}

void dae::BTGameMode::OnSceneLoaded(Scene*, size_t)
{
	m_Ingredients = 0;

	CreateGrid();

	if (m_pGrid)
	{
		m_pGrid->GetOnGridMappedDelegate() += [this]()
		{
			m_MaxIngredients = 0;
			m_pGrid->ForEachSpawner([this](BTTile& tile)
				{
					if (tile.pSpawner && BurgerTime::IsIngredient(static_cast<BurgerTime::SpawnID>(tile.pSpawner->GetSpawnID())))
						++m_MaxIngredients;
				});

			RespawnAll();
			m_pHUD = CreateHUD();
		};
	}
}

void dae::BTGameMode::OnPlayerLostLife()
{
	StartRound();
}

void dae::BTGameMode::OnPlayerDeath()
{
	auto& gameState{ dae::GameState::GetInstance() };
	gameState.SetGameMode(std::make_shared<dae::MainMenuGameMode>());
}

void dae::BTGameMode::AddIngredient()
{
	++m_Ingredients;
	if (m_Ingredients == m_MaxIngredients)
	{
		OpenNextLevel();
	}
}

void dae::BTGameMode::LoadLevels()
{
	m_ScenesIndices.push_back(SceneManager::GetInstance().LoadScene("Scenes/BurgerTimeLevel1NoBurger.scene"));
	m_ScenesIndices.push_back(SceneManager::GetInstance().LoadScene("Scenes/BurgerTimeLevel1.scene"));
	m_ScenesIndices.push_back(SceneManager::GetInstance().LoadScene("Scenes/BurgerTimeLevel2.scene"));

	if (m_CurrentLevel >= m_ScenesIndices.size())
		m_CurrentLevel = 0;
}

void dae::BTGameMode::OpenNextLevel()
{
	++m_CurrentLevel;
	if (m_CurrentLevel == m_ScenesIndices.size())
		m_CurrentLevel = 0;
	SceneManager::GetInstance().GetOnSceneLoaded() += std::bind(&BTGameMode::OnSceneLoaded, this, std::placeholders::_1, std::placeholders::_2);
	SceneManager::GetInstance().OpenSceneByIndex(m_ScenesIndices[m_CurrentLevel]);

	OnExit();
	OnEnter();
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

void dae::BTGameMode::RespawnAllActiveCharacters()
{
	m_pGrid->ForEachSpawner([](BTTile& tile)
		{
			if (tile.pSpawner->GetInstance() && tile.pSpawner->GetInstance()->IsActive()
				&& (tile.pSpawner->GetSpawnID() == static_cast<uint32_t>(BurgerTime::SpawnID::Player)
				|| tile.pSpawner->GetSpawnID() == static_cast<uint32_t>(BurgerTime::SpawnID::MrHotDog))
				|| tile.pSpawner->GetSpawnID() == static_cast<uint32_t>(BurgerTime::SpawnID::MrEgg)
				|| tile.pSpawner->GetSpawnID() == static_cast<uint32_t>(BurgerTime::SpawnID::MrPickle))
				tile.pSpawner->Spawn();
		});
}

void dae::BTGameMode::RespawnAll()
{
	m_pGrid->ForEachSpawner([](BTTile& tile)
		{
			tile.pSpawner->Spawn();
		});
}

void dae::BTGameMode::SpawnAllPlayers()
{
	m_pGrid->ForEachSpawner([](BTTile& tile)
		{
			if (tile.pSpawner->GetSpawnID() == static_cast<uint32_t>(BurgerTime::SpawnID::Player))
			{
				tile.pSpawner->Spawn();
			}
		});
}

void dae::BTGameMode::SpawnPlayer(size_t index)
{
	size_t i{};
	m_pGrid->ForEachSpawner([=, &i](BTTile& tile)
		{
			if (tile.pSpawner->GetSpawnID() == static_cast<uint32_t>(BurgerTime::SpawnID::Player))
			{
				if ((i + 1) == index)
					tile.pSpawner->Spawn();
				++i;
			}
		});
}
