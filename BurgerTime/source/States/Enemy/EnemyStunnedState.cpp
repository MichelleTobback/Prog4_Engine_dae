#include "EnemyStunnedState.h"
#include "Core/Time.h"
#include "Components/EnemyComponent.h"

dae::EnemyStunnedState::EnemyStunnedState(EnemyComponent* pEnemy, float duration)
    : EnemyState(pEnemy), m_Duration{duration}
{
}

void dae::EnemyStunnedState::OnEnter()
{
    m_Time = 0.f;
    GetCharacter().pAnimator->SetAnimState(CharacterAnimationController::CharacterAnim::Stunned);
}

dae::State::StatePtr dae::EnemyStunnedState::OnUpdate()
{
    if (GetEnemy()->GetCharacter()->Get().pHealth->GetValue() == 0)
        return GetEnemy()->GetStates().pDieState.get();
    if (GetEnemy()->GetOverlappedBurger() && GetEnemy()->GetOverlappedBurger()->GetVelocity().y > 0.1f)
        return GetEnemy()->GetStates().pFallState.get();

    m_Time += Time::GetInstance().GetDeltaTime();
    if (m_Time > m_Duration)
        return GetEnemy()->GetStates().pGoToPlayerState.get();
    return this;
}

void dae::EnemyStunnedState::OnExit()
{
    GetCharacter().pAnimator->SetAnimState(CharacterAnimationController::CharacterAnim::WalkDown);
}
