#include "ComponentSystem.h"

void dae::ComponentSystem::Update()
{
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

void dae::ComponentSystem::BroadcastMessage(const ComponentMessage& msg)
{
	for (auto& component : m_Components)
	{
		component.second->ReceiveMessage(msg);
	}
}
