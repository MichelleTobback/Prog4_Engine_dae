#include "InputComponent.h"

uint8_t dae::InputComponent::s_InstanceCount{ 0 };

dae::InputComponent::InputComponent(GameObject* pGameObject)
	: Component(pGameObject), m_InputId{ s_InstanceCount++ }
{
}

dae::InputComponent::~InputComponent()
{
	--s_InstanceCount;
}
