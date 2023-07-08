#include "ComponentSystem.h"

dae::ComponentSystem::~ComponentSystem()
{
	for (auto& component : m_Components)
	{
		component.second->OnDestroy();
	}
}

void dae::ComponentSystem::Awake()
{
	for (auto& component : m_Components)
	{
		component.second->Awake();
	}
}

void dae::ComponentSystem::Sleep()
{
	for (auto& component : m_Components)
	{
		component.second->Sleep();
	}
}

void dae::ComponentSystem::Update()
{
	while (!m_pComponentsToAwake.empty())
	{
		m_pComponentsToAwake.front()->Awake();
		m_pComponentsToAwake.pop();
	}

	for (auto& component : m_Components)
	{
		component.second->Update();
	}
}

void dae::ComponentSystem::FixedUpdate()
{
	for (auto& component : m_Components)
	{
		component.second->FixedUpdate();
	}
}

void dae::ComponentSystem::LateUpdate()
{
	for (auto& component : m_Components)
	{
		component.second->LateUpdate();
	}
}

void dae::ComponentSystem::ForEach(const ComponentFunc& fn) const
{
	for (auto& component : m_Components)
	{
		fn(component.second.get());
	}
}

size_t dae::ComponentSystem::Count() const
{
	return m_Components.size();
}

void dae::ComponentSystem::Clear()
{
	for (auto& component : m_Components)
	{
		component.second->Sleep();
		component.second->OnDestroy();
	}
	m_Components.clear();
}

void dae::ComponentSystem::BroadcastMessage(const ComponentMessage& msg)
{
	for (auto& component : m_Components)
	{
		component.second->ReceiveMessage(msg);
	}
}

bool dae::ComponentSystem::HasComponent(const std::string& typeName) const
{
	return m_Components.find(typeName.c_str()) != m_Components.end();
}
