#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Core/Singleton.h"
#include "Scene/Serializable.h"
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

		OnDeserializedComponentDelegate& GetOnDeserializedComponentDelegate();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
	};
}
