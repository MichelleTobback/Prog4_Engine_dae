#include "InputHandlerComponent.h"

dae::InputHandlerComponent::InputHandlerComponent(GameObject* pOwner)
	: Component(pOwner), m_pCommandHandler{std::make_unique<InputCommand::ICHandler>()}
{
}

void dae::InputHandlerComponent::Update()
{
	m_pCommandHandler->Handle();
}

dae::InputCommand::ICHandler& dae::InputHandlerComponent::GetHandler()
{
	return *m_pCommandHandler;
}
