#pragma once
#include "Core/UUID.h"
#include "Managers/SceneManager.h"

#include <stack>
#include <glm/glm.hpp>
#include <unordered_map>

namespace dae
{
	class GameObject;
	using GameObjectContainer = std::unordered_map<UUID, std::shared_ptr<GameObject>>;

	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		GameObject* Add(std::shared_ptr<GameObject> object);

		GameObject* Instantiate(GameObject* pParent = nullptr, const glm::vec3& pos = {});
		GameObject* Instantiate(UUID uuid, GameObject* pParent = nullptr, const glm::vec3& pos = {});

		void Update();
		void FixedUpdate();
		void LateUpdate();
		void HandleObjectLifeTime();

		inline const GameObjectContainer& GetObjects() const { return m_Objects; }
		GameObject* GetGameObject(UUID uuid);

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		friend class SceneSerializer;
		explicit Scene(const std::string& name);

		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		std::string m_Name;
		GameObjectContainer m_Objects{};

		std::stack<UUID> m_ObjectsPendingDestroy{};

		static unsigned int m_IdCounter; 
	};

}
