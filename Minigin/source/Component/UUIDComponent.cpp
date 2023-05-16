#include "UUIDComponent.h"

dae::UUIDComponent::UUIDComponent(GameObject* pOwner, UUID uuid)
	: Component(pOwner)
	, m_Uuid{uuid}
{
}
