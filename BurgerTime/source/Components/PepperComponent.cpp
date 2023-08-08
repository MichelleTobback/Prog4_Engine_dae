#include "PepperComponent.h"
#include "BurgerTime.h"
#include "Scene/GameObject.h"
#include "Component/SpriteAnimatorComponent.h"
#include "BurgerTime.h"
#include "Components/EnemyComponent.h"

dae::PepperComponent::PepperComponent(GameObject* pOwner, SpriteAnimatorComponent* pAnimator, SpriteAtlasComponent* pSpriteAtlas, RigidBody2DComponent* pRigidBody)
	: Component(pOwner), m_pAnimator{pAnimator}
{
	size_t clip{ BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, glm::vec4{11.f, 2, 3, 1}) };
	pAnimator->GetClip(clip).GetOnClipEndDelegate() += std::bind(&PepperComponent::OnAnimEnd, this);
	pAnimator->SetClip(clip);

	pRigidBody->GetOnBeginOverlap() += std::bind(&PepperComponent::OnBeginOverlap, this, std::placeholders::_1);
}

void dae::PepperComponent::Awake()
{

	m_pAnimator->PlayFromStart();
}

void dae::PepperComponent::Update()
{
}

void dae::PepperComponent::OnBeginOverlap(const CollisionHit& hit)
{
	if (hit.pOtherCollider->GetCollisionLayer() == BurgerTime::ENEMY_COLLISION_LAYER)
	{
		EnemyComponent* pEnemy{ hit.pOther->GetComponent<EnemyComponent>() };
		pEnemy->Stun();
	}
}

void dae::PepperComponent::OnAnimEnd()
{
	GetOwner()->Destroy();
}
