#include "Component.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"

dae::Component::Component(GameObject* pGameObject)
	: m_pOwner{ pGameObject }
{

}


dae::TransformComponent& dae::Component::GetTransform()
{
	return GetOwner()->GetTransform();
}

dae::UUID dae::Component::GetUUID() const
{
	return GetOwner()->GetUUID();
}

dae::Scene* dae::Component::GetScene() const
{
	return GetOwner()->GetScene();
}

std::string dae::Component::GetName() const
{
	return typeid(*this).name();
}
