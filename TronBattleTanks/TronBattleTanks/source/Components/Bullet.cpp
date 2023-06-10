#include "Bullet.h"
#include "Component/Physics/RigidBody2DComponent.h"
#include "Scene/GameObject.h"
#include "Component/TransformComponent.h"
#include "Component/CircleComponent.h"

#include "Prefabs.h"

dae::Bullet::Bullet(GameObject* pOwner)
	: Component(pOwner)
{
	
}

void dae::Bullet::Awake()
{
	m_pRigidBody = GetOwner()->AddComponent<RigidBody2DComponent>(CollisionMode::Dynamic);
	Prefab::CreateTextureRendererObject(GetOwner()->GetScene(), "Sprites/Bullet.png")->AttachToGameObject(GetOwner());
	m_pRigidBody->ApplyForce(GetTransform().GetForward() * 500.f);
	auto pCircle{ GetOwner()->AddComponent<CircleComponent>(6.f) };
	auto pCollider{ GetOwner()->AddComponent<CircleColliderComponent>() };
	pCollider->SetShape(pCircle);
	m_pRigidBody->AddCollider(pCollider);
	m_pRigidBody->SetMass(0.1f);
	m_pRigidBody->SetRestitution(1.f);
}
