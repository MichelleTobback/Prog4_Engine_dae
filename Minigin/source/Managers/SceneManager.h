#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Core/Singleton.h"
#include "Scene/Serializable.h"
#include "Scene/ComponentFactory.h"
#include <filesystem>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		Scene& LoadScene(const std::filesystem::path& file);
		void SaveScene(Scene& scene, const std::filesystem::path& file);

		void Update();
		void FixedUpdate();
		void LateUpdate();
		void HandleObjectsLifeTime();

		Scene* GetCurrent() { return m_scenes[0].get(); }

		ComponentFactory* GetComponentFactory();

	private:
		friend class Singleton<SceneManager>;
		SceneManager();
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::unique_ptr<ComponentFactory> m_pComponentFactory;
	};
}
