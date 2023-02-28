#pragma once
#include <memory>

#include "ComponentSystem.h"

namespace dae
{
	typedef uint32_t GameObjectID;

	class GameObject final
	{
	public:
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;

		void BroadcastComponentMessage(const ComponentMessage& msg);

		GameObjectID GetID();

		inline bool IsValid() const { return m_IsValid; }

		template <typename T, typename ... TArgs>
		T& AddComponent(TArgs&& ... args)
		{
			return m_ComponentSystem.AddComponent<T>(std::forward<TArgs>(args)...);
		}

		template <typename T>
		bool RemoveComponent()
		{
			return m_ComponentSystem.RemoveComponent<T>();
		}

		template <typename T>
		const T& GetComponent() const
		{
			return m_ComponentSystem.GetComponent<T>();
		}

		template <typename T>
		T& GetComponent()
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

		GameObject(GameObjectID id);
		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:

		GameObjectID m_Id;
		ComponentSystem m_ComponentSystem{};
		bool m_IsValid{ true };
	};
}
