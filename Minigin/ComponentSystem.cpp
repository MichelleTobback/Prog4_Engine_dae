#include "ComponentSystem.h"

void dae::ComponentSystem::Update(GameObject* pGameObject)
{
	for (auto& component : m_Components)
	{
		component.second->Update(pGameObject);
	}
}

void dae::ComponentSystem::FixedUpdate(GameObject* pGameObject)
{
	for (auto& component : m_Components)
	{
		component.second->FixedUpdate(pGameObject);
	}
}

void dae::ComponentSystem::LateUpdate(GameObject* pGameObject)
{
	for (auto& component : m_Components)
	{
		component.second->LateUpdate(pGameObject);
	}
}

void dae::ComponentSystem::BroadcastMessage(const ComponentMessage& msg)
{
	for (auto& component : m_Components)
	{
		component.second->ReceiveMessage(msg);
	}
}
