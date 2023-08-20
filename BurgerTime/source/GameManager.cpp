#include "GameManager.h"
#include "Managers/SceneManager.h"
#include "Scene/Scene.h"

dae::GameManager::GameManager()
	: m_pStateMachine{std::make_unique<StackedStateMachine<GameState, Scene&>>()}
{
	GameDataSerializer gameDataSerializer{};
	gameDataSerializer.Deserialize(m_SaveData);

	if (!m_SaveData.names.size())
		m_SaveData.names.push_back(" ");
	m_HiScore = m_SaveData.hiScores.front().hiScore;
}

dae::GameManager::~GameManager()
{
	GameDataSerializer gameDataSerializer{};
	gameDataSerializer.Serialize(m_SaveData);
}

dae::GameState& dae::GameManager::GetState() const
{
	return m_pStateMachine->GetCurrent();
}

void dae::GameManager::PushState(const std::shared_ptr<GameState>& pState)
{
	SceneManager& sceneManager{ SceneManager::GetInstance() };
	Scene* pCurrentScene{ sceneManager.GetCurrent() };
	m_PrevScenes.push(sceneManager.GetCurrentIndex());
	if (sceneManager.GetCurrentIndex() != pState->GetSceneIndex())
	{
		sceneManager.GetOnSceneLoaded() += [this, pState, pCurrentScene](Scene* pScene, size_t)
		{
			m_pStateMachine->Push(pState, *pCurrentScene, *pScene);
		};
		sceneManager.OpenSceneByIndex(pState->GetSceneIndex());
	}
	else
	{
		sceneManager.GetCurrent()->Sleep();
		m_pStateMachine->Push(pState, *pCurrentScene, *pCurrentScene);
		sceneManager.GetCurrent()->Awake();
	}
}

void dae::GameManager::PopState(int amount)
{
	SceneManager& sceneManager{ SceneManager::GetInstance() };
	Scene* pCurrentScene{ sceneManager.GetCurrent() };
	for (int i{1}; i < amount; ++i)
	{
		if (!m_PrevScenes.empty())
			m_PrevScenes.pop();
	}
	size_t prevSceneIdx{ m_PrevScenes.top() };
	m_PrevScenes.pop();

	if (sceneManager.GetCurrentIndex() != prevSceneIdx)
	{
		sceneManager.GetOnSceneLoaded() += [this, amount, pCurrentScene](Scene* pScene, size_t)
		{
			m_pStateMachine->Pop(*pCurrentScene, *pScene, amount);
		};
		sceneManager.OpenSceneByIndex(prevSceneIdx);
	}
	else
	{
		sceneManager.GetCurrent()->Sleep();
		m_pStateMachine->Pop(*pCurrentScene, *pCurrentScene, amount);
		sceneManager.GetCurrent()->Awake();
	}
}

void dae::GameManager::SetState(const std::shared_ptr<GameState>&)
{
	
}

void dae::GameManager::PopWhile(const std::function<bool(size_t)>& fn)
{
	SceneManager& sceneManager{ SceneManager::GetInstance() };
	Scene* pCurrentScene{ sceneManager.GetCurrent() };
	size_t currentIndex{ sceneManager.GetCurrentIndex() };
	size_t prevSceneIdx{ m_PrevScenes.top() };
	int amount{};
	while (!m_PrevScenes.empty() && fn(prevSceneIdx))
	{
		m_PrevScenes.pop();
		currentIndex = prevSceneIdx;
		prevSceneIdx = m_PrevScenes.top();
		++amount;
	}

	if (sceneManager.GetCurrentIndex() != currentIndex)
	{
		sceneManager.GetOnSceneLoaded() += [this, amount, pCurrentScene](Scene* pScene, size_t)
		{
			m_pStateMachine->Pop(*pCurrentScene, *pScene, amount);
		};
		sceneManager.OpenSceneByIndex(currentIndex);
	}
	else
	{
		sceneManager.GetCurrent()->Sleep();
		m_pStateMachine->Pop(*pCurrentScene, *pCurrentScene, amount);
		sceneManager.GetCurrent()->Awake();
	}
}

void dae::GameManager::SetPlayerName(const std::string& name)
{
	m_PlayerName = name;
}

const std::string& dae::GameManager::GetPlayerName()
{
	return m_PlayerName;
}

void dae::GameManager::UpdateHiScores(uint32_t score)
{
	auto hiScoreIt = std::find_if(m_SaveData.hiScores.begin(), m_SaveData.hiScores.end(),[score](SaveGame::HighScore& hiScore) 
		{
			return score > hiScore.hiScore || !hiScore.nameIndex;
		});

	if (hiScoreIt != m_SaveData.hiScores.end())
	{
		auto& hiScoreToRemove = m_SaveData.hiScores.back();
		hiScoreToRemove.hiScore = score;

		int nameCount{ static_cast<int>(std::count_if(m_SaveData.hiScores.begin(), m_SaveData.hiScores.end(),
			[&hiScoreToRemove](const SaveGame::HighScore& s) 
			{
				return s.nameIndex == hiScoreToRemove.nameIndex;
			}))};

		size_t nameIdx = m_SaveData.names.size();
		if (nameCount > 1)
			m_SaveData.names.push_back(m_PlayerName);
		else
		{
			nameIdx = hiScoreToRemove.nameIndex;
			m_SaveData.names[nameIdx] = m_PlayerName;
		}

		hiScoreToRemove.nameIndex = static_cast<unsigned char>(nameIdx);

		std::sort(m_SaveData.hiScores.begin(), m_SaveData.hiScores.end(),
			[](const SaveGame::HighScore& hi0, const SaveGame::HighScore& hi1) 
			{
				return hi0.hiScore > hi1.hiScore;
			});

		m_HiScore.GetOnValueChangedDelegate().Clear();
		m_HiScore = m_SaveData.hiScores.front().hiScore;
	}
}
