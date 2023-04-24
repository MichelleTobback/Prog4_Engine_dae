#pragma once
#include "Managers/SceneManager.h"

#include <stack>

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);

		void Update();
		void FixedUpdate();
		void LateUpdate();
		void HandleObjectLifeTime();

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		std::string m_Name;
		std::vector <std::shared_ptr<GameObject>> m_Objects{};

		std::stack<size_t> m_ObjectsPendingDestroy{};

		static unsigned int m_IdCounter; 
	};

}
