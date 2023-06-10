#include "TankCanon.h"
#include "Bullet.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"
#include "Prefabs.h"

dae::TankCanon::TankCanon(GameObject* pOwner)
	: Component(pOwner)
{
	auto pTankCenter{ pOwner->GetScene()->Instantiate(GetOwner()->GetParent(), { 18.f, 18.f, 0.f }) };
	m_pSocket = pOwner->GetScene()->Instantiate(pTankCenter, {18.f, 0.f, 0.f});
	
	auto pVisual{ Prefab::CreateTextureRendererObject(GetOwner()->GetScene(), "Sprites/TankCanon.png") };
	pVisual->AttachToGameObject(GetOwner());

	m_pShootSound = std::make_unique<AudioClip>("Sounds/shoot.wav");

	//debug
	auto pDebug{ Prefab::CreateTextureRendererObject(GetOwner()->GetScene(), "Sprites/Bullet.png") };
	pDebug->AttachToGameObject(m_pSocket);
}

void dae::TankCanon::Update()
{
	m_pSocket->GetParent()->GetTransform().SetLocalRotation(GetTransform().GetLocalRotation());
}

void dae::TankCanon::Shoot()
{
	auto pBullet{ GetOwner()->GetScene()->Instantiate(nullptr, m_pSocket->GetTransform().GetWorldPosition())->AddComponent<Bullet>() };
	pBullet->GetTransform().Rotate(GetTransform().GetWorldRotationAngle());

	m_pShootSound->Play();
}

