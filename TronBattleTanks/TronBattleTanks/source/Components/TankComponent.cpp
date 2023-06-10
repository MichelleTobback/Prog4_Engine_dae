#include "TankComponent.h"
#include "HealthComponent.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"
#include "Components/MovementComponent.h"
#include "Component/Physics/RigidBody2DComponent.h"
#include "Core/Time.h"

dae::TankComponent::TankComponent(GameObject* pOwner, const TankDesc& desc)
	: Component(pOwner)
{
	auto pScene{ GetOwner()->GetScene() };
	m_pMovement = GetOwner()->AddComponent<MovementComponent>();

	//Body
	m_pTankBody = pScene->Instantiate(GetOwner());
	//sprite
	m_pTankBody->AddComponent<TextureComponent>()->SetTexture("Sprites/TankBody.png");
	m_pTankBody->AddComponent<TextureRenderComponent>();

	//Canon
	auto pCanon{ pScene->Instantiate(GetOwner()) };
	m_pTankCanon = pCanon->AddComponent<TankCanon>();

	//physics
	auto pCollider{ GetOwner()->AddComponent<BoxCollider2DComponent>() };
	auto pShape{ GetOwner()->AddComponent<QuadComponent>(glm::vec2{32.f, 32.f}) };
	pCollider->SetShape(pShape);
	GetRigidBody().AddCollider(pCollider);

	m_pHealth = GetOwner()->AddComponent<HealthComponent>(desc.health);
}

dae::TankComponent::~TankComponent()
{
}

void dae::TankComponent::Awake()
{
	
}

void dae::TankComponent::Move(const glm::vec2& dir)
{
	m_pMovement->Move({ dir.x, dir.y });
	m_pTankBody->GetTransform().SetLocalRotation(glm::degrees(glm::atan(dir.y, dir.x)));
}

void dae::TankComponent::RotateCanon(float dir)
{
	float dt{ Time::GetInstance().GetDeltaTime() };
	float speed{ m_RotationSpeed * dt * dir };
	m_pTankCanon->GetTransform().Rotate(speed);
}
