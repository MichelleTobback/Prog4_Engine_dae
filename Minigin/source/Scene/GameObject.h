#pragma once
#include <memory>

#include "Component/UUIDComponent.h"
#include "Component/ComponentSystem.h"
#include "Component/TransformComponent.h"
#include "Core/UUID.h"

namespace dae
{
	typedef uint32_t GameObjectID;

	class Scene;
	class GameObject final
	{
	public:
		GameObject(Scene* pScene);
		GameObject(Scene* pScene, UUID uuid);
		virtual ~GameObject() = default;

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update();
		void FixedUpdate();
		void LateUpdate();

		void BroadcastComponentMessage(const ComponentMessage& msg);

		void AttachToGameObject(GameObject* pParent, bool keepWorld = false);
		void DetachGameObject(GameObject* pChild);

		Scene* GetScene() { return m_pScene; }

		inline bool IsValid() const { return m_IsValid; }
		bool IsRoot() const;

		template <typename T, typename ... TArgs>
		T* AddComponent(TArgs&& ... args)
		{
			T* pComponent{ m_ComponentSystem.AddComponent<T>(this, std::forward<TArgs>(args)...) };
			return pComponent;
		}

		template <typename T>
		bool RemoveComponent()
		{
			return m_ComponentSystem.RemoveComponent<T>();
		}

		template <typename T>
		const T* GetComponent() const
		{
			return m_ComponentSystem.GetComponent<T>();
		}

		template <typename T>
		T* GetComponent()
		{
			return m_ComponentSystem.GetComponent<T>();
		}

		template <typename T>
		bool HasComponent() const
		{
			return m_ComponentSystem.HasComponent<T>();
		}

		void Destroy();

		inline GameObject* GetParent() const { return m_pParent; }
		inline const std::vector<GameObject*>& GetChildren() const { return m_pChildren; }
		inline TransformComponent& GetTransform() { return *m_pTransformComponent; }
		inline UUID GetUUID() const { return m_pUUID->GetUUID(); }
		inline const ComponentSystem& GetComponentSystem() const { return m_ComponentSystem; }

	private:
		inline void AddChild(GameObject* pChild) { m_pChildren.push_back(pChild); }
		void RemoveChild(GameObject* pChild);

		ComponentSystem m_ComponentSystem{};
		bool m_IsValid{ true };

		GameObject* m_pParent{nullptr};
		std::vector<GameObject*> m_pChildren{};

		TransformComponent* m_pTransformComponent{ nullptr };
		UUIDComponent* m_pUUID{ nullptr };
		Scene* m_pScene;
	};
}
