#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Core/Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update();
		void FixedUpdate();
		void LateUpdate();
		void HandleObjectsLifeTime();

		Scene* GetCurrent() { return m_scenes[0].get(); }

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
	};
}
