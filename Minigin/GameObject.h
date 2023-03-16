#pragma once
#include <memory>

#include "ComponentSystem.h"

namespace dae
{
	typedef uint32_t GameObjectID;

	class TransformComponent;
	class GameObject final
	{
	public:
		GameObject();
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

		inline bool IsValid() const { return m_IsValid; }

		template <typename T, typename ... TArgs>
		T* AddComponent(TArgs&& ... args)
		{
			return m_ComponentSystem.AddComponent<T>(this, std::forward<TArgs>(args)...);
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

		inline void Destroy()
		{
			m_IsValid = false;
		}

		inline GameObject* GetParent() const { return m_pParent; }
		inline const std::vector<GameObject*>& GetChildren() const { return m_pChildren; }
		TransformComponent& GetTransform() { return *m_pTransformComponent; }

	private:

		inline void AddChild(GameObject* pChild) { m_pChildren.push_back(pChild); }
		void RemoveChild(GameObject* pChild);

		ComponentSystem m_ComponentSystem{};
		bool m_IsValid{ true };

		GameObject* m_pParent{nullptr};
		std::vector<GameObject*> m_pChildren{};

		TransformComponent* m_pTransformComponent{ nullptr };
	};
}
