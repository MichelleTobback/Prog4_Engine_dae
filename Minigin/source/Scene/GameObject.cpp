#include <string>
#include "GameObject.h"
#include "Managers/ResourceManager.h"
#include "Renderer/Renderer.h"

#include "Component/RenderComponent.h"

#include "Scene/Scene.h"


dae::GameObject::GameObject(Scene* pScene)
	: GameObject(pScene, UUID())
{
	
}

dae::GameObject::GameObject(Scene* pScene, UUID uuid)
	: m_pScene{ pScene }
{
	m_pTransformComponent = AddComponent<TransformComponent>();
	m_pUUID = AddComponent<UUIDComponent>(uuid);
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

void dae::GameObject::BroadcastComponentMessage(const ComponentMessage& msg)
{
	m_ComponentSystem.BroadcastMessage(msg);
}

void dae::GameObject::AttachToGameObject(GameObject* pParent, bool keepWorld)
{
	if (m_pParent)
		m_pParent->RemoveChild(this);

	pParent->AddChild(this);
	m_pParent = pParent;

	if (m_pParent == nullptr)
		m_pTransformComponent->GetLocalPosition();
	else
	{
		if (keepWorld)
		{
			auto& parentTransform{ pParent->GetTransform() };
			glm::vec3 localPos{ m_pTransformComponent->GetLocalPosition() - parentTransform.GetWorldPosition()};
			m_pTransformComponent->SetLocalPosition(localPos);
		}
		m_pTransformComponent->SetDirty(TransformComponent::TransformFlag::Position, true);
		m_pTransformComponent->SetDirty(TransformComponent::TransformFlag::Rotation, true);
	}
}

void dae::GameObject::DetachGameObject(GameObject* pChild)
{
	pChild->m_pParent = nullptr;
	RemoveChild(pChild);
	pChild->GetTransform().SetLocalPosition(GetTransform().GetWorldPosition());
}

bool dae::GameObject::IsRoot() const
{
	return GetParent() == nullptr;
}

void dae::GameObject::RemoveChild(GameObject* pChild)
{
	m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), pChild), m_pChildren.end());
}

void dae::GameObject::Destroy()
{
	m_IsValid = false;

	for (GameObject* pChild : m_pChildren)
	{
		pChild->Destroy();
	}
}
