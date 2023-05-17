#include "TankCanon.h"
#include "Bullet.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"
#include "Prefabs.h"

dae::TankCanon::TankCanon(GameObject* pOwner)
	: Component(pOwner)
{
	m_pSocket = pOwner->GetScene()->Instantiate(GetOwner());
	m_pSocket->GetTransform().Translate({ 16.f, 16.f, 0.f });
	Prefab::CreateTextureRendererObject(GetOwner()->GetScene(), "Sprites/TankCanon.png")->AttachToGameObject(pOwner);

	m_pShootSound = std::make_unique<AudioClip>("Sounds/shoot.wav");
}

void dae::TankCanon::Shoot()
{
	auto pBullet{ GetOwner()->GetScene()->Instantiate(nullptr, m_pSocket->GetTransform().GetWorldPosition())->AddComponent<Bullet>() };
	pBullet->GetOwner()->GetTransform().Rotate(GetOwner()->GetTransform().GetWorldRotation());

	m_pShootSound->Play();
}

