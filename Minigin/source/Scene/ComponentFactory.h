#pragma once
#include "Core/UUID.h"
#include "Scene/GameObject.h"
#include "Serializable.h"
#include <functional>
#include <unordered_map>

namespace dae
{
	class Component;
	using InstantiateComponentFn = std::function<Component* (DeserializeParams&)>;
	class ComponentFactory final
	{
	public:
		ComponentFactory();
		~ComponentFactory();

		template<typename T>
		SerializableType RegisterComponent();
		template<typename T>
		bool IsRegistered() const;
		void ForEachRegisteredComponent(const std::function<void(const SerializableType&, Serializable*)>& fn) const;

		Serializable* GetComponentSerializer(ComponentID id) const;
		Component* InstantiateComponent(ComponentID id, DeserializeParams& params);
		ComponentID GetID(const std::string& componentName);
		bool IsRegistered(ComponentID id) const;
		bool IsRegistered(const std::string& name) const;
		size_t GetRegisteredCount() const;

	private:
		std::unordered_map<ComponentID, std::unique_ptr<Serializable>> m_Components{};
		std::unordered_map<std::string, ComponentID> m_NamesToIds{};
	};

	template<typename T>
	dae::SerializableType ComponentFactory::RegisterComponent()
	{
		auto pSerializable{std::make_unique<T>()};
		SerializableType info{ pSerializable->GetTypeInfo() };
		m_NamesToIds.emplace(info.componentName, info.id);
		m_Components.emplace(info.id, std::move(pSerializable));
		return info;
	}

	template<typename T>
	inline bool ComponentFactory::IsRegistered() const
	{
		ComponentID id{ typeid(T).hash_code() };
		return IsRegistered(id);
	}
}