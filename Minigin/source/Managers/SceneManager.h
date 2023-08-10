#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Core/Singleton.h"
#include "Scene/Serializable.h"
#include "Scene/ComponentFactory.h"
#include "Core/Delegate.h"

#include <filesystem>

namespace dae
{
	class Scene;
	using SceneDelegate = Delegate<void(Scene*, size_t index)>;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name, bool openOnLoad);
		Scene& OpenSceneByIndex(size_t index);
		Scene& OpenSceneByIndex(size_t index, const std::function<void(Scene& scene)>& fnOnLoaded);
		size_t LoadScene(const std::filesystem::path& file, bool openOnLoad = false);
		void SaveScene(Scene& scene, const std::filesystem::path& file);

		void Update();
		void FixedUpdate();
		void LateUpdate();
		void HandleObjectsLifeTime();

		Scene* GetCurrent() { return m_scenes[m_CurrentSceneIndex].get(); }
		size_t GetCount()const { return m_scenes.size(); }
		size_t GetCurrentIndex()const { return m_CurrentSceneIndex; }

		ComponentFactory* GetComponentFactory();
		SceneDelegate& GetOnSceneLoaded();
		SceneDelegate& GetOnSceneUnloaded();

	private:
		friend class Singleton<SceneManager>;
		SceneManager();
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<Scene> m_ActiveScene{ nullptr };
		std::unique_ptr<ComponentFactory> m_pComponentFactory;
		size_t m_CurrentSceneIndex{ 0 };
		size_t m_NextScene{ 0 };
		std::function<void(Scene& scene)> m_fnOnSceneLoaded{nullptr};
		std::unique_ptr<SceneDelegate> m_pOnSceneLoaded;
		std::unique_ptr<SceneDelegate> m_pOnSceneUnloaded;
	};
}
