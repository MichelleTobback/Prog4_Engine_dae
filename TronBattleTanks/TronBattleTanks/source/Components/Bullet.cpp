#include "Bullet.h"
#include "Components/MovementComponent.h"
#include "Scene/GameObject.h"

#include "Prefabs.h"

dae::Bullet::Bullet(GameObject* pOwner)
	: Component(pOwner)
{
	m_pMovement = pOwner->AddComponent<MovementComponent>();
	Prefab::CreateTextureRendererObject(GetOwner()->GetScene(), "Sprites/Bullet.png")->AttachToGameObject(GetOwner());
}

void dae::Bullet::Update()
{
	m_pMovement->Move(GetOwner()->GetTransform().GetForward());
}
