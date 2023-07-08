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
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);

	if (!m_ActiveScene)
		m_ActiveScene = m_scenes.back();

	return *scene;
}

dae::Scene& dae::SceneManager::LoadSceneByIndex(size_t index)
{
	assert(index < m_scenes.size() && "no valid scene index");

	m_NextScene = index;
	return *m_scenes[index];
}

dae::Scene& dae::SceneManager::LoadScene(const std::filesystem::path& file)
{
	const std::string name{ file.filename().stem().string()};
	Scene& scene{ CreateScene(name) };

	if (file.extension().string()._Equal(".scene"))
	{
		static SceneSerializer deserializer{};
		deserializer.Deserialize(&scene, file);
	}
	else
	{
		std::cout << "scene could not be loaded from location " << file.string() << "!\nMake sure file has extention .scene\n";
	}

	return scene;
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
}

dae::SceneDelegate& dae::SceneManager::GetOnSceneLoaded()
{
	return *m_pOnSceneLoaded;
}
dae::SceneDelegate& dae::SceneManager::GetOnSceneUnloaded()
{
	return *m_pOnSceneUnloaded;
}
