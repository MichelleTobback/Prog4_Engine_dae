#include "ComponentFactory.h"
#include "Core/BinaryReader.h"

dae::ComponentFactory::ComponentFactory()
{
}

dae::ComponentFactory::~ComponentFactory()
{
}

dae::Serializable* dae::ComponentFactory::GetComponentSerializer(ComponentID id) const
{
	return m_Components.at(id).get();
}

dae::Component* dae::ComponentFactory::InstantiateComponent(ComponentID id, DeserializeParams& params)
{
	assert(IsRegistered(id) && "Component not registered!");
	assert(params.pGameObject != nullptr && "Cant instantiate component for non-existing GameObject!");

	return m_Components[id]->Deserialize(params);
}

dae::ComponentID dae::ComponentFactory::GetID(const std::string& componentName)
{
	return m_NamesToIds.at(componentName);
}

bool dae::ComponentFactory::IsRegistered(ComponentID id) const
{
	return m_Components.find(id) != m_Components.end();
}

bool dae::ComponentFactory::IsRegistered(const std::string& name) const
{
	return m_NamesToIds.find(name) != m_NamesToIds.end();
}

size_t dae::ComponentFactory::GetRegisteredCount() const
{
	return m_Components.size();
}

void dae::ComponentFactory::ForEachRegisteredComponent(const std::function<void(const SerializableType&, Serializable*)>& fn) const
{
	for (const auto& pair : m_NamesToIds)
	{
		fn(SerializableType{ pair.first, pair.second }, m_Components.at(pair.second).get());
	}
}


