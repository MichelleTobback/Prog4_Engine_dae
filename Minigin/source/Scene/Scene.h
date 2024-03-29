#pragma once
#include "Core/UUID.h"
#include "Managers/SceneManager.h"
#include "Managers/PhysicsSystem.h"

#include <queue>
#include <glm/glm.hpp>
#include <unordered_map>

namespace dae
{
	class GameObject;
	using GameObjectContainer = std::unordered_map<UUID, std::shared_ptr<GameObject>>;

	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name, bool openOnLoad);
	public:
		GameObject* Add(std::shared_ptr<GameObject> object);

		GameObject* Instantiate(GameObject* pParent = nullptr, const glm::vec3& pos = {});
		GameObject* Instantiate(UUID uuid, GameObject* pParent = nullptr, const glm::vec3& pos = {});

		void Awake();
		void Sleep();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void HandleObjectLifeTime();

		inline const GameObjectContainer& GetObjects() const { return m_Objects; }
		GameObject* GetGameObject(UUID uuid);
		std::vector<GameObject*> GetGameObjectWithTag(const std::string& tag);
		std::vector<GameObject*> GetGameObjectsAtPos(const glm::vec3& pos, float epsilon = 1.f, bool onlyRoot = true);

		PhysicsScene& GetPhysics() { return *m_pPhysics; }
		void DestroyGameObject(GameObject* pObject);

		const std::string& GetName() const { return m_Name; }

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

		std::queue<UUID> m_ObjectsPendingDestroy{};

		std::unique_ptr<PhysicsScene> m_pPhysics;
		bool m_Running{false};

		static unsigned int m_IdCounter; 
	};

}
