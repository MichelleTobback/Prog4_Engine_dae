#include "Component.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"

dae::Component::Component(GameObject* pGameObject)
	: m_pOwner{ pGameObject }
{

}


dae::UUID dae::Component::GetUUID() const
{
	return GetOwner()->GetUUID();
}

dae::Scene* dae::Component::GetScene() const
{
	return GetOwner()->GetScene();
}
