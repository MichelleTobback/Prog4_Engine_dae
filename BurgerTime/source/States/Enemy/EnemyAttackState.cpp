#include "EnemyAttackState.h"
#include "Components/EnemyComponent.h"
#include "Core/Time.h"


dae::EnemyAttackState::EnemyAttackState(EnemyComponent* pEnemy, float cooldown)
    : EnemyState(pEnemy), m_Cooldown{cooldown}
{
}

void dae::EnemyAttackState::OnEnter()
{
    m_Time = 0.f;
    if (GetEnemy()->GetOverlappedPlayer())
        GetEnemy()->GetOverlappedPlayer()->Get().pHealth->DealDamage(1);
}

dae::State::StatePtr dae::EnemyAttackState::OnUpdate()
{
    m_Time += Time::GetInstance().GetDeltaTime();
    if (m_Time > m_Cooldown)
        return GetEnemy()->GetStates().pGoToPlayerState.get();
    return this;
}

void dae::EnemyAttackState::OnExit()
{
}
