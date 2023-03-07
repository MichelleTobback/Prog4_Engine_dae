#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include "RenderComponent.h"
#include "TransformComponent.h"


dae::GameObject::GameObject()
{
	m_pTransformComponent = AddComponent<TransformComponent>();
}

void dae::GameObject::Update()
{
	m_ComponentSystem.Update();
}

void dae::GameObject::FixedUpdate()
{
	m_ComponentSystem.FixedUpdate();
}

void dae::GameObject::LateUpdate()
{
	m_ComponentSystem.LateUpdate();
}

void dae::GameObject::Render() const
{
	if (HasComponent<TextureRenderComponent>())
		GetComponent<TextureRenderComponent>()->Render();
}

void dae::GameObject::BroadcastComponentMessage(const ComponentMessage& msg)
{
	m_ComponentSystem.BroadcastMessage(msg);
}

void dae::GameObject::AttachToGameObject(GameObject* pParent, bool keepWorld)
{
	pParent->AddChild(this);
	m_pParent = pParent;

	if (m_pParent == nullptr)
		m_pTransformComponent->GetLocalPosition();
	else
	{
		if (keepWorld)
		{
			auto pParentTransform{ pParent->GetComponent<TransformComponent>() };
			glm::vec3 localPos{ m_pTransformComponent->GetLocalPosition() - pParentTransform->GetWorldPosition()};
			m_pTransformComponent->SetLocalPosition(localPos);
		}
		m_pTransformComponent->m_IsPositionDirty = true;
	}
}

void dae::GameObject::DetachGameObject(GameObject* pChild)
{
	m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), pChild), m_pChildren.end());
}
