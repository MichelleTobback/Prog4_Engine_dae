#pragma once

#include "Component.h"

#include <unordered_map>
#include <memory>
#include <functional>

#include <assert.h>
#include <queue>

namespace dae
{
	class GameObject;

	constexpr size_t MAX_COMPONENTS = 32;
	typedef std::unordered_map<std::string, std::shared_ptr<Component>> ComponentMap;

	class ComponentSystem final
	{
	public:
		void Update();
		void FixedUpdate();
		void LateUpdate();

		bool HasComponent(const std::string& typeName) const;
		template <typename T>
		bool HasComponent() const
		{
			return HasComponent(Component::GetName<T>());
		}

		template <typename T>
		const T* GetComponent() const
		{
			assert(HasComponent<T>() && "Invalid Component!");

			return m_Components.at(Component::GetName<T>())->As<T>();
		}

		template <typename T>
		T* GetComponent()
		{
			assert(HasComponent<T>() && "Invalid Component!");
			return m_Components.at(Component::GetName<T>())->As<T>();
		}

		template <typename T, typename ... TArgs>
		T* AddComponent(TArgs&& ... args)
		{
			const std::string typeName{ Component::GetName<T>() };
			m_Components.insert(std::make_pair(typeName, std::make_shared<T>(std::forward<TArgs>(args)...)));
			auto pComponent{ m_Components.at(typeName) };
			m_pComponentsToAwake.push(pComponent.get());
			return pComponent->As<T>();
		}

		template <typename T>
		bool RemoveComponent()
		{
			if (!HasComponent<T>())
				return false;

			m_Components.erase(Component::GetName<T>());
			return true;
		}

		using ComponentFunc = std::function<void(Component*)>;
		void ForEach(const ComponentFunc& fn) const;
		size_t Count() const;

		void BroadcastMessage(const ComponentMessage& msg);

	private:
		friend class SceneSerializer;
		ComponentMap m_Components;

		std::queue<Component*> m_pComponentsToAwake{};

	};
}
