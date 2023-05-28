#include "GridLevelComponent.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"

dae::GridLevelComponent::GridLevelComponent(GameObject* pOwner, const GridLevelDesc& desc)
	: Component(pOwner)
	, m_Desc{ desc }
{
	
}
