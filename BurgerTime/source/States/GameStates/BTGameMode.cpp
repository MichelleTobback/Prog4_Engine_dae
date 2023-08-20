#include "BTGameMode.h"
#include "Managers/SceneManager.h"
#include "Scene/Scene.h"
#include "Components/ObjectSpawner.h"
#include "Components/HealthComponent.h"
#include "Components/PlayerController.h"
#include "Components/CharacterInfo.h"
#include "Component/CameraComponent.h"
#include "BurgerTime.h"
#include "States/GameStates/MainMenuGameMode.h"
#include "States/GameStates/HiScoreGameState.h"
#include "GameManager.h"
#include "Prefabs.h"

#include "Managers/ServiceLocator.h"

dae::ObservableType<uint32_t> dae::BTGameMode::m_Score{};

dae::BTGameMode::BTGameMode(size_t lvlIdx)
 : GameState(lvlIdx), m_CurrentLevel{ 0 }, m_ScenesIndices{2, 3, 4}
{
	for (size_t i{}; i < m_ScenesIndices.size(); ++i)
	{
		if (m_ScenesIndices[i] == lvlIdx)
		{
			m_CurrentLevel = i;
			break;
		}
	}

	m_MusicId = ServiceLocator::GetSoundSystem().AddMusic("Sounds/02 Game Music.mp3");
}

void dae::BTGameMode::OnEnter(Scene& scene)
{
	ServiceLocator::GetSoundSystem().PlayMusic(m_MusicId, 90.f, -1);

	m_Ingredients = 0;
	if (m_CurrentLevel == 0)
	{
		m_Score.GetOnValueChangedDelegate() += [](uint32_t value) 
		{
			auto& hiScore{ GameManager::GetInstance().GetHiScore() };
			if (value > hiScore())
				hiScore = value;
		};
		m_Score = 0;
	}

	for (auto spawn : m_pPlayerSpawns)
	{
		if (spawn->pSpawner && spawn->pSpawner->GetInstance())
			spawn->pSpawner->GetInstance()->Destroy();
	}
	m_pPlayerSpawns.clear();

	CreateGrid(scene);
	if (m_pGrid)
	{
		m_MaxIngredients = 0;
		m_pGrid->ForEachSpawner([this](BTTile& tile)
			{
				if (tile.pSpawner && BurgerTime::IsIngredient(static_cast<BurgerTime::SpawnID>(tile.pSpawner->GetSpawnID())))
					++m_MaxIngredients;
				else if (tile.pSpawner && static_cast<BurgerTime::SpawnID>(tile.pSpawner->GetSpawnID()) == BurgerTime::SpawnID::Player)
				{
					m_pPlayerSpawns.push_back(&tile);
				}
			});

		RespawnAll();
	}

	m_pHUD = CreateHUD(scene);

	m_pCamera = scene.Instantiate(0u, nullptr)->AddComponent<dae::CameraComponent>(0.5f);
	m_pCamera->GetTransform().Translate(glm::vec3{ 24.f, 64.f, 0.f });
	m_pCamera->SetCurrent();
}

void dae::BTGameMode::OnExit(Scene&)
{
	ServiceLocator::GetSoundSystem().StopMusic();
	m_Score.GetOnValueChangedDelegate().Clear();
	GameManager::GetInstance().UpdateHiScores(m_Score());

	m_pCamera->GetOwner()->Destroy();
	if (m_pGrid)
	{
		m_pGrid->GetOnGridMappedDelegate().Clear();
	}
	m_pPlayerSpawns.clear();
	if (m_pHUD)
		m_pHUD->Destroy();
	m_pHUD = nullptr;
	m_pGrid = nullptr;
}

void dae::BTGameMode::OnSceneLoaded(Scene*, size_t)
{
	
}

void dae::BTGameMode::OnPlayerLostLife()
{
	StartRound();
}

void dae::BTGameMode::OnPlayerDeath()
{
	auto& gameManager{ dae::GameManager::GetInstance() };
	gameManager.PushState(std::make_unique<HiScoreGameState>());
}

void dae::BTGameMode::AddIngredient()
{
	++m_Ingredients;
	if (m_Ingredients == m_MaxIngredients)
	{
		OpenNextLevel();
	}
}

glm::vec3 dae::BTGameMode::GetClosestPlayerPos(const glm::vec3& pos) const
{
	glm::vec3 closestPos{};
	float closestDistance{FLT_MAX};
	for (const auto& playerSpawn : m_pPlayerSpawns)
	{
		GameObject* pPlayer{ playerSpawn->pSpawner->GetInstance() };
		if (!pPlayer)
			continue;
		const glm::vec3 currentPos{ pPlayer->GetTransform().GetWorldPosition() };
		const float distance{ glm::distance2(currentPos, pos) };
		if (distance < closestDistance)
		{
			closestDistance = distance;
			closestPos = currentPos;
		}
	}
	return closestPos;
}

dae::GameObject* dae::BTGameMode::CreatePlayer(dae::Scene* pScene)
{
	return Prefabs::CreatePeterPepper(pScene);
}

void dae::BTGameMode::LoadLevels()
{
	if (m_CurrentLevel >= m_ScenesIndices.size())
		m_CurrentLevel = 0;
}

void dae::BTGameMode::OpenNextLevel()
{
	++m_CurrentLevel;
	if (m_CurrentLevel == m_ScenesIndices.size())
		m_CurrentLevel = 0;

	SceneManager::GetInstance().GetOnSceneLoaded() += [this](Scene* pScene, size_t)
	{
		OnExit(*pScene);
		OnEnter(*pScene);
	};
	SceneManager::GetInstance().OpenSceneByIndex(m_ScenesIndices[m_CurrentLevel]);
}

void dae::BTGameMode::CreateGrid(Scene& scene)
{
	auto pObjects{ scene.GetGameObjectWithTag("Lvl")};
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
			{
				tile.pSpawner->Spawn();
			}
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

void dae::BTGameMode::RespawnAfterDuration(GameObject* pObject, float duration)
{
	m_pGrid->ForEachSpawner([pObject, duration](BTTile& tile)
		{
			if (tile.pSpawner->GetInstance() == pObject)
			{
				tile.pSpawner->SpawnDelayed(duration);
			}
		});
}
