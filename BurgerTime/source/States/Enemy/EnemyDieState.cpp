#include "EnemyDieState.h"
#include "Components/CharacterInfo.h"
#include "Scene/GameObject.h"
#include "Components/EnemyComponent.h"
#include "States/GameStates/BTGameMode.h"
#include "GameManager.h"

dae::EnemyDieState::EnemyDieState(EnemyComponent* pEnemy)
    : EnemyState(pEnemy), m_pDieSound{ std::make_unique<AudioClip>("Sounds/15_hit.wav") }
{
}

void dae::EnemyDieState::OnEnter()
{
    auto& character{ GetEnemy()->GetCharacter()->Get() };
    auto pRigidBody{ character.pController->GetRigidBody() };
    for (auto& pCollider : pRigidBody->GetColliders())
    {
        m_Layer = pCollider->GetCollisionLayer();
        pCollider->SetCollisionLayer(CollisionLayer::None);
    }
    m_pDieSound->Play();
}

dae::State::StatePtr dae::EnemyDieState::OnUpdate()
{
    auto& character{ GetEnemy()->GetCharacter()->Get() };
    auto pAnimator{ character.pAnimator->GetAnimator() };
    const size_t lastFrame{ pAnimator->GetCurrentClip().frames.size() - 1 };
    if (pAnimator->GetCurrentFrame() == lastFrame)
    {
        GetEnemy()->GetOwner()->SetActive(false);
        return GetEnemy()->GetStates().pGoToPlayerState.get();
    }
    return this;
}

void dae::EnemyDieState::OnExit()
{
    auto& character{ GetEnemy()->GetCharacter()->Get() };
    auto pRigidBody{ character.pController->GetRigidBody() };
    for (auto& pCollider : pRigidBody->GetColliders())
    {
        pCollider->SetCollisionLayer(m_Layer);
    }

    BTGameMode* pGameMode{ dynamic_cast<BTGameMode*>(&GameManager::GetInstance().GetState()) };
    if (pGameMode)
        pGameMode->RespawnAfterDuration(pRigidBody->GetOwner(), 5.f);
}
