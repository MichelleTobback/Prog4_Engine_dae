#include "SceneManager.h"
#include "Scene/Scene.h"

#include "Scene/SceneSerializer.h"
#include <iostream>

void dae::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::FixedUpdate()
{
	for (auto& scene : m_scenes)
	{
		scene->FixedUpdate();
	}
}

void dae::SceneManager::LateUpdate()
{
	for (auto& scene : m_scenes)
	{
		scene->LateUpdate();
	}
}

void dae::SceneManager::HandleObjectsLifeTime()
{
	for (const auto& scene : m_scenes)
	{
		scene->HandleObjectLifeTime();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
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
