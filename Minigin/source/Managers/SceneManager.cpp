#include "SceneManager.h"
#include "Scene/Scene.h"

#include "Scene/SceneSerializer.h"
#include <iostream>

void dae::SceneManager::Update()
{
	m_ActiveScene->Update();
}

void dae::SceneManager::FixedUpdate()
{
	m_ActiveScene->FixedUpdate();
}

void dae::SceneManager::LateUpdate()
{
	m_ActiveScene->LateUpdate();
	HandleObjectsLifeTime();
}

void dae::SceneManager::HandleObjectsLifeTime()
{
	m_ActiveScene->HandleObjectLifeTime();

	if (m_NextScene != m_CurrentSceneIndex)
	{
		if (m_pOnSceneUnloaded)
		{
			m_pOnSceneUnloaded->Invoke(m_ActiveScene.get(), m_CurrentSceneIndex);
			m_pOnSceneUnloaded->Clear();
		}
		m_ActiveScene->Sleep();
		m_CurrentSceneIndex = m_NextScene;
		m_ActiveScene = m_scenes[m_CurrentSceneIndex];
		m_ActiveScene->Awake();
		if (m_pOnSceneLoaded)
		{
			m_pOnSceneLoaded->Invoke(m_ActiveScene.get(), m_CurrentSceneIndex);
			m_pOnSceneLoaded->Clear();
		}
		if (m_fnOnSceneLoaded)
		{
			m_fnOnSceneLoaded(*m_ActiveScene);
			m_fnOnSceneLoaded = nullptr;
		}
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name, bool openOnLoad)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);

	if (!m_ActiveScene)
		m_ActiveScene = m_scenes.back();
	if (openOnLoad)
		m_NextScene = (m_scenes.size() - 1);

	return *scene;
}

dae::Scene& dae::SceneManager::OpenSceneByIndex(size_t index)
{
	assert(index < m_scenes.size() && "no valid scene index");

	m_NextScene = index;
	return *m_scenes[index];
}

dae::Scene& dae::SceneManager::OpenSceneByIndex(size_t index, const std::function<void(Scene& scene)>& fnOnLoaded)
{
	m_fnOnSceneLoaded = fnOnLoaded;
	return OpenSceneByIndex(index);
}

size_t dae::SceneManager::LoadScene(const std::filesystem::path& file, bool openOnLoad)
{
	assert(file.extension().string()._Equal(".scene") && "Scene file must have .scene extension");

	const std::string name{ file.filename().stem().string()};

	for (size_t i{}; i < m_scenes.size(); ++i)
	{
		if (m_scenes[i]->GetName()._Equal(name))
		{
			if (openOnLoad)
			{
				m_NextScene = i;
				m_fnOnSceneLoaded = nullptr;
			}

			return i;
		}
	}

	Scene& scene{ CreateScene(name, openOnLoad) };

	static SceneSerializer deserializer{};
	deserializer.Deserialize(&scene, file);

	return (m_scenes.size() - 1);
}

void dae::SceneManager::SaveScene(Scene& scene, const std::filesystem::path& file)
{
	if (file.extension().string()._Equal(".scene"))
	{
		static SceneSerializer serializer{};
		serializer.Serialize(&scene, file);
	}
}

dae::ComponentFactory* dae::SceneManager::GetComponentFactory()
{
	return m_pComponentFactory.get();
}

dae::SceneManager::SceneManager()
	: m_pComponentFactory{std::make_unique<ComponentFactory>()}
	, m_pOnSceneLoaded{ std::make_unique<SceneDelegate>() }
	, m_pOnSceneUnloaded{ std::make_unique<SceneDelegate>() }
{
	SceneSerializer::RegisterEngineComponents(*m_pComponentFactory);
	CreateScene("empty", true);
}

dae::SceneDelegate& dae::SceneManager::GetOnSceneLoaded()
{
	return *m_pOnSceneLoaded;
}
dae::SceneDelegate& dae::SceneManager::GetOnSceneUnloaded()
{
	return *m_pOnSceneUnloaded;
}
