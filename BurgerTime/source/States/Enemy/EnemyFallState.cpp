#include "EnemyFallState.h"

#include "Components/CharacterInfo.h"
#include "Components/EnemyComponent.h"
#include "Scene/GameObject.h"
#include "Components/EnemyComponent.h"
#include "BurgerTime.h"
#include "Scene/Scene.h"
#include "Core/BitFlag.h"

#include <iostream>

dae::EnemyFallState::EnemyFallState(EnemyComponent* pEnemy)
    : EnemyState(pEnemy)
{
}

void dae::EnemyFallState::OnEnter()
{
    auto pRigidBody{ GetCharacter().pController->GetRigidBody()};
    m_OriginalSpeed = GetCharacter().pController->GetMovementSpeed();
    const glm::vec3& newVel{ GetEnemy()->GetOverlappedBurger()->GetVelocity() };
    GetCharacter().pController->SetMovementSpeed(newVel.y);
    pRigidBody->GetCollider(0)->SetTrigger(true);
    pRigidBody->SetVelociy(newVel);
}

dae::State::StatePtr dae::EnemyFallState::OnUpdate()
{
    auto pRigidBody{ GetEnemy()->GetOverlappedBurger() };
    if (!pRigidBody || pRigidBody->GetVelocity().y < 0.001f)
        return GetEnemy()->GetStates().pGoToPlayerState.get();

    GetCharacter().pController->Move({ 0.f, 1.f });
    return this;
}

void dae::EnemyFallState::OnExit()
{
    GetCharacter().pController->SetMovementSpeed(m_OriginalSpeed);
}

void dae::EnemyFallState::OnEndOverlap(const CollisionHit& hit)
{
    if (hit.pOtherCollider->GetCollisionLayer() == BurgerTime::LEVEL_COLLISION_LAYER)
    {
        auto pRigidBody{ GetCharacter().pController->GetRigidBody() };
        pRigidBody->GetCollider(0)->SetTrigger(false);
    }
}
