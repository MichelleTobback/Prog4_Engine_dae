#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include "RenderComponent.h"

dae::GameObject::~GameObject() = default;

dae::GameObject::GameObject(GameObjectID id)
	: m_Id{id}
{

}

void dae::GameObject::Update()
{
	m_ComponentSystem.Update(this);
}

void dae::GameObject::FixedUpdate()
{
	m_ComponentSystem.FixedUpdate(this);
}

void dae::GameObject::LateUpdate()
{
	m_ComponentSystem.LateUpdate(this);
}

void dae::GameObject::Render() const
{
	if (HasComponent<TextureRenderComponent>())
		GetComponent<TextureRenderComponent>().Render(this);
}

void dae::GameObject::BroadcastComponentMessage(const ComponentMessage& msg)
{
	m_ComponentSystem.BroadcastMessage(msg);
}

dae::GameObjectID dae::GameObject::GetID()
{
	return m_Id;
}
