#pragma once

#include "Component.h"

#include <unordered_map>
#include <memory>

#include <assert.h>

namespace dae
{
	class GameObject;

	constexpr size_t MAX_COMPONENTS = 32;
	typedef std::unordered_map<const char*, std::shared_ptr<Component>> ComponentMap;

	class ComponentSystem final
	{
	public:
		void Update();
		void FixedUpdate();
		void LateUpdate();

		template <typename T>
		bool HasComponent() const
		{
			const char* typeName{ typeid(T).name() };
			bool hasComponent{ m_Components.find(typeName) != m_Components.end() };

			return hasComponent;
		}

		template <typename T>
		const T* GetComponent() const
		{
			assert(HasComponent<T>() && "Invalid Component!");

			const char* typeName{ typeid(T).name() };
			return m_Components.at(typeName)->As<T>();
		}

		template <typename T>
		T* GetComponent()
		{
			assert(HasComponent<T>() && "Invalid Component!");

			const char* typeName{ typeid(T).name() };
			return m_Components.at(typeName)->As<T>();
		}

		template <typename T, typename ... TArgs>
		T* AddComponent(TArgs&& ... args)
		{
			const char* typeName{ typeid(T).name() };
			m_Components.insert(std::make_pair(typeName, std::make_shared<T>(std::forward<TArgs>(args)...)));

			return m_Components.at(typeName)->As<T>();
		}

		template <typename T>
		bool RemoveComponent()
		{
			if (!HasComponent<T>())
				return false;

			const char* typeName{ typeid(T).name() };
			m_Components.erase(typeName);
		}

		void BroadcastMessage(const ComponentMessage& msg);

	private:

		ComponentMap m_Components;
	};
}
