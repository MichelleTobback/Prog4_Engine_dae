#include "Component.h"
#include "Scene/GameObject.h"

dae::Component::Component(GameObject* pGameObject)
	: m_pOwner{ pGameObject }
{

}


dae::UUID dae::Component::GetUUID() const
{
	return GetOwner()->GetUUID();
}
