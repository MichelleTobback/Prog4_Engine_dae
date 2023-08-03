#include "EnemyComponent.h"
#include "Scene/GameObject.h"

#include "HealthComponent.h"

#include "State/GameState/GameState.h"
#include "States/GameModes/BTGameMode.h"
#include "BurgerTime.h"

dae::EnemyComponent::EnemyComponent(GameObject* pOwner, uint32_t reward)
	: Component(pOwner), m_Reward{reward}
{
}

void dae::EnemyComponent::Awake()
{
	RigidBody2DComponent* pRigidBody{ GetOwner()->GetComponent<RigidBody2DComponent>() };
    if (pRigidBody)
	    pRigidBody->GetOnBeginOverlap() += std::bind(&EnemyComponent::OnOverlap, this, std::placeholders::_1);

	m_pHealth = GetOwner()->GetComponent<HealthComponent>();
    if (m_pHealth)
        m_pHealth->GetOnDeath() += std::bind(&EnemyComponent::OnDeath, this);

    m_pCurrentGameMode = dynamic_cast<BTGameMode*>(&GameState::GetInstance().GetGameMode());
}

void dae::EnemyComponent::OnDeath()
{
    m_pCurrentGameMode->GetScore() += m_Reward;
    GetOwner()->SetActive(false);
}

void dae::EnemyComponent::OnOverlap(const CollisionHit& hit)
{
    CollisionLayer overlappedLayer{ hit.pOtherCollider->GetCollisionLayer() };
    if (overlappedLayer == BurgerTime::PLAYER_COLLISION_LAYER)
    {
        hit.pOther->GetComponent<HealthComponent>()->DealDamage(1);
    }
    else if (overlappedLayer == BurgerTime::INGREDIENT_COLLISION_LAYER
        && hit.pOtherCollider->GetRigidBody()->GetVelocity().y > 0.f)
    {
        if (m_pHealth)
            m_pHealth->DealDamage(1);
    }
}
