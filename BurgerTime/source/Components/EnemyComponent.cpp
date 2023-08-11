#include "EnemyComponent.h"
#include "Scene/GameObject.h"

#include "HealthComponent.h"

#include "Components/CharacterInfo.h"

#include "State/GameState/GameState.h"
#include "States/GameModes/BTGameMode.h"
#include "BurgerTime.h"

#include "States/Enemy/EnemyGoToPlayerState.h"
#include "States/Enemy/EnemyClimbLadderState.h"
#include "States/Enemy/EnemyDieState.h"
#include "States/Enemy/EnemyFallState.h"
#include "States/Enemy/EnemyAttackState.h"
#include "States/Enemy/EnemyStunnedState.h"

#include "Components/BurgerIngredient.h"

dae::EnemyComponent::EnemyComponent(GameObject* pOwner, CharacterInfoComponent* pCharacter, uint32_t reward)
	: Component(pOwner), m_pCharacter{pCharacter}, m_Reward{reward}
{
    m_States.pGoToPlayerState = std::make_unique<EnemyGoToPlayerState>(this);
    m_States.pClimbLadderState = std::make_unique<EnemyClimbLadderState>(this);
    m_States.pDieState = std::make_unique<EnemyDieState>(this);
    m_States.pFallState = std::make_unique<EnemyFallState>(this);
    m_States.pAttackState = std::make_unique<EnemyAttackState>(this);
    m_States.pStunnedState = std::make_unique<EnemyStunnedState>(this, 3.f);
}

void dae::EnemyComponent::Awake()
{
    m_pStateMachine = GetOwner()->GetComponent<StateMachine>();

	RigidBody2DComponent* pRigidBody{ m_pCharacter->Get().pController->GetRigidBody() };
    if (pRigidBody)
    {
        pRigidBody->GetOnBeginOverlap() += std::bind(&EnemyComponent::OnOverlap, this, std::placeholders::_1);
        pRigidBody->GetOnEndOverlap() += std::bind(&EnemyComponent::OnEndOverlap, this, std::placeholders::_1);
    }

    if (m_pCharacter->Get().pHealth)
        m_pCharacter->Get().pHealth->GetOnDeath() += std::bind(&EnemyComponent::OnDeath, this);

    m_pCurrentGameMode = dynamic_cast<BTGameMode*>(&GameState::GetInstance().GetGameMode());
}

void dae::EnemyComponent::Stun()
{
    m_pStateMachine->SetState(m_States.pStunnedState.get());
}

void dae::EnemyComponent::OnDeath()
{
    m_pCurrentGameMode->GetScore() += m_Reward;
}

void dae::EnemyComponent::OnOverlap(const CollisionHit& hit)
{
    CollisionLayer overlappedLayer{ hit.pOtherCollider->GetCollisionLayer() };
    if (overlappedLayer == BurgerTime::INGREDIENT_COLLISION_LAYER
        && hit.pOtherCollider->GetRigidBody()->GetVelocity().y > 0.1f)
    {
        if (hit.pOther->GetTransform().GetWorldPosition().y + 2.f > GetTransform().GetWorldPosition().y)
            m_pOverlappedBurger = hit.pOtherCollider->GetRigidBody();
        else
            GetCharacter()->Get().pHealth->DealDamage(1);
    }
    else if (overlappedLayer == BurgerTime::PLAYER_COLLISION_LAYER)
    {
        m_pOverlappedPlayer = hit.pOther->GetComponent<CharacterInfoComponent>();
    }

    dynamic_cast<EnemyState*>(&m_pStateMachine->GetCurrent())->OnBeginOverlap(hit);
}

void dae::EnemyComponent::OnEndOverlap(const CollisionHit& hit)
{
    CollisionLayer overlappedLayer{ hit.pOtherCollider->GetCollisionLayer() };
    if (overlappedLayer == BurgerTime::INGREDIENT_COLLISION_LAYER)
    {
        m_pOverlappedBurger = nullptr;
    }
    else if (overlappedLayer == BurgerTime::LEVEL_COLLISION_LAYER)
    {
        hit.pCollider->GetRigidBody()->GetCollider(0)->SetTrigger(false);
    }
    else if (overlappedLayer == BurgerTime::PLAYER_COLLISION_LAYER)
    {
        m_pOverlappedPlayer = nullptr;
    }

    dynamic_cast<EnemyState*>(&m_pStateMachine->GetCurrent())->OnEndOverlap(hit);
}
