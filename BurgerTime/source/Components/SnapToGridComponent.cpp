#include "SnapToGridComponent.h"
#include "Component/TransformComponent.h"

#include "State/GameState/GameState.h"
#include "States/GameModes/BTGameMode.h"

dae::SnapToGridComponent::SnapToGridComponent(GameObject* pOwner, RigidBody2DComponent* pRigidBody)
	: Component(pOwner), m_pRigidBody{pRigidBody}
{
}

void dae::SnapToGridComponent::Awake()
{
	m_pCurrentGameMode = dynamic_cast<BTGameMode*>(&GameState::GetInstance().GetGameMode());
}

void dae::SnapToGridComponent::Update()
{
	bool snap{ false };
	glm::vec3 pos{ GetTransform().GetWorldPosition() };
	const glm::vec3& tilePos{ m_pCurrentGameMode->GetGrid().GetTile(pos).pos };
	const glm::vec3& vel{ m_pRigidBody->GetVelocity() };
	const float epsilon{ 2.f };
	if (std::abs(tilePos.x - pos.x) < epsilon && std::abs(vel.x) < epsilon)
	{
		pos.x = tilePos.x;
		snap = true;
	}
	if (std::abs(tilePos.y - pos.y) < epsilon && std::abs(vel.y) < epsilon)
	{
		pos.y = tilePos.y;
		snap = true;
	}
	if (snap)
		GetTransform().SetLocalPosition(pos);
}
